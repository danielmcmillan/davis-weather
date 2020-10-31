import { loop1Definition } from "./loopPackets/loop1";
import { loop2Definition } from "./loopPackets/loop2";
import { parsePacket } from "./parsePacket";
import { validateCRC } from "./validateCRC";

const testData =
  "4c4f4f0001ff7fc674e00236130200ff32010000000000003b01ff7fff7f2500ff36ff34003500ff000000ffff7f0000ffff00000000000000000000020000dfff886c886cdf74ff0116000314051029140808ff7fff7fff7fff7fff7fff7f0a0db9e0";
console.log(validateCRC(Buffer.from(testData, "hex")));
console.log(parsePacket(Buffer.from(testData, "hex"), loop2Definition));
