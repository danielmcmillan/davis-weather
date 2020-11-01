import { mockLoop2 } from "./mock/loop2";
import { validateCRC } from "./validateCRC";

describe("validateCRC", () => {
  it("should return true for valid data", () => {
    expect(validateCRC(mockLoop2)).toBe(true);
  });

  it("should return false for malformed data", () => {
    const broken = Buffer.alloc(mockLoop2.length);
    mockLoop2.copy(broken);
    broken.writeInt16LE(123, 10);
    expect(validateCRC(broken)).toBe(false);
  });
});
