#!/usr/bin/env node
import AWS from "aws-sdk";
import { createGzip } from "zlib";

async function main() {
  const sqsQueueName = process.env.SQS_QUEUE!;
  const s3BucketName = process.env.S3_BUCKET!;
  const s3 = new AWS.S3();
  const sqs = new AWS.SQS();

  // Get the SQS queue URL
  const sqsQueueUrl = (
    await sqs
      .getQueueUrl({
        QueueName: sqsQueueName,
      })
      .promise()
  ).QueueUrl;
  if (!sqsQueueUrl) {
    throw new Error(`No URL for SQS queue ${sqsQueueName}`);
  }
  // Check the S3 bucket is accessible
  await s3.headBucket({ Bucket: s3BucketName }).promise();

  const key = `loopgz/${new Date().toISOString().replace(/:/g, ".")}.gz`;
  console.log(
    `Archiving from queue ${sqsQueueUrl} to S3 bucket ${s3BucketName} key ${key}.`
  );

  // Upload to S3 from a Gzip stream
  const stream = createGzip({});
  const s3Promise = s3
    .upload({
      Bucket: s3BucketName,
      Key: key,
      Body: stream,
    })
    .promise();

  // Write all messages from the queue to the stream
  const messagesToDelete: string[] = [];
  for await (const message of getSQSIterator(sqs, sqsQueueUrl)) {
    if (stream.write(message.message + "\n", "utf8")) {
      messagesToDelete.push(message.receiptHandle);
    }
    if (process.stdout.clearLine) {
      process.stdout.clearLine(0);
      process.stdout.cursorTo(0);
      process.stdout.write(`Messages received: ${messagesToDelete.length}`);
    }
  }
  console.log();

  // Close the stream and wait for upload
  stream.end();
  await s3Promise;

  console.log(`Successfully archived ${messagesToDelete.length} messages.`);

  // Delete messages
  await deleteMessages(sqs, sqsQueueUrl, messagesToDelete);
}

interface SQSMessage {
  message: string;
  receiptHandle: string;
}

async function* getSQSIterator(
  sqs: AWS.SQS,
  queueUrl: string
): AsyncIterableIterator<SQSMessage> {
  while (true) {
    const result = await sqs
      .receiveMessage({
        QueueUrl: queueUrl,
        MaxNumberOfMessages: 10,
        WaitTimeSeconds: 0,
      })
      .promise();
    if (result.Messages !== undefined && result.Messages.length > 0) {
      for (const message of result.Messages) {
        if (message.Body && message.ReceiptHandle) {
          yield { message: message.Body, receiptHandle: message.ReceiptHandle };
        }
      }
    } else {
      return;
    }
  }
}

async function deleteMessages(sqs: AWS.SQS, queueUrl: string, receiptHandles: string[]) {
  while (receiptHandles.length > 0) {
    const batch = receiptHandles.splice(0, 10);
    await sqs
      .deleteMessageBatch({
        QueueUrl: queueUrl,
        Entries: batch.map((receiptHandle, index) => ({
          Id: String(index),
          ReceiptHandle: receiptHandle,
        })),
      })
      .promise();
    if (process.stdout.clearLine) {
      process.stdout.clearLine(0);
      process.stdout.cursorTo(0);
      process.stdout.write(`Messages remaining to delete: ${receiptHandles.length}`);
    }
  }
  console.log();
}

main().catch((err) => {
  console.error(err);
  process.exit(1);
});
