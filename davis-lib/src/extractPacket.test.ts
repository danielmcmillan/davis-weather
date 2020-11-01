import { extractPacket } from "./extractPacket";

describe("extractPacket", () => {
  const testTimestamp = 1604217605;

  it("should work for the test message format", () => {
    const { data, timestamp } = extractPacket(
      "Hello number 123:f0" + "00".repeat(97) + "0f",
      {
        sentTimestamp: testTimestamp,
      }
    );
    expect(timestamp).toBe(testTimestamp);
    expect(data.length).toBe(99);
    expect(data.readUInt8(0)).toBe(0xf0);
    expect(data.readUInt8(98)).toBe(0x0f);
  });
});
