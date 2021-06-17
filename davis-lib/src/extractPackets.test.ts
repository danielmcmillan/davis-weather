import { extractPackets } from "./extractPackets";

describe("extractPacket", () => {
  const testTimestamp = 1604217605;

  it("should work for message v01", () => {
    const [{ data, timestamp }] = extractPackets(
      "0160c9d85300004c4f4fec01ff7fcf74c30236230200ff39011c00150006003b01ff7fff7f3100ff50ff37003700ff000000ffff7f0000ffff02000000010000000200020000dfff896c896ce074ff060b0a1500081338060909ff7fff7fff7fff7fff7fff7f0a0dcdd5",
      {
        sentTimestamp: testTimestamp,
      }
    )!;
    expect(timestamp).toBe(0x60c9d853 * 1000);
    expect(data.length).toBe(99);
    expect(data.readUInt8(0)).toBe(0x4c);
    expect(data.readUInt8(98)).toBe(0xd5);
  });
});
