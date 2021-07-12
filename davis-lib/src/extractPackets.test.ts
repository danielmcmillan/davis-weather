import { extractPackets } from "./extractPackets";

describe("extractPacket", () => {
  const testTimestamp = 1604217605;

  it("should work for message v01", () => {
    const [{ data, statusData, timestamp }] = extractPackets(
      "0160c9d85300004c4f4fec01ff7fcf74c30236230200ff39011c00150006003b01ff7fff7f3100ff50ff37003700ff000000ffff7f0000ffff02000000010000000200020000dfff896c896ce074ff060b0a1500081338060909ff7fff7fff7fff7fff7fff7f0a0dcdd5",
      {
        sentTimestamp: testTimestamp,
      }
    )!;
    expect(timestamp).toBe(0x60c9d853 * 1000);
    expect(data.length).toBe(99);
    expect(data.readUInt8(0)).toBe(0x4c);
    expect(data.readUInt8(98)).toBe(0xd5);
    expect(statusData).toBeUndefined();
  });

  it("should work for message with status data", () => {
    const [{ data, statusData, timestamp }] = extractPackets(
      "0160ec01a500004c4f4f1401ff7fa7759a0239e70103ff67002200290008005a00ff7fff7f2f00ff5eff31003000ff000000ffff7f0000ffff01000000000000000100020000dfff406d406da275ff0016021302020132070909ff7fff7fff7fff7fff7fff7f0a0d175e0b520ce4",
      {
        sentTimestamp: testTimestamp,
      }
    )!;
    expect(timestamp).toBe(0x60ec01a5 * 1000);
    expect(data.length).toBe(99);
    expect(statusData?.length).toBe(4);
    expect(data.readUInt8(0)).toBe(0x4c);
    expect(data.readUInt8(98)).toBe(0x5e);
    expect(statusData?.readUInt16BE(0)).toBe(0x0b52);
    expect(statusData?.readUInt16BE(2)).toBe(0x0ce4);
  });
});
