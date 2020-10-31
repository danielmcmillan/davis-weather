export interface PacketDefinition<T extends {}> {
  fields: PacketField<T, keyof T>[];
}

export enum IntFieldFormat {
  Int8,
  UInt8,
  Int16LE,
  Int16BE,
  UInt16LE,
  UInt16BE,
}

interface BasePacketField<T extends {}, I extends keyof T> {
  id: I;
  /** Byte offset within the packet. */
  offset: number;
  /** Optional function to determine whether the field data is unavailable. If true is returned value will be null. */
  isUnavailable?: (data: Buffer) => boolean;
}
interface PacketIntField<T extends {}, I extends keyof T> extends BasePacketField<T, I> {
  intFormat: IntFieldFormat;
  intTransform: (value: number) => T[I];
}
interface PacketBytesField<T extends {}, I extends keyof T>
  extends BasePacketField<T, I> {
  byteArrayLength: number;
  byteArrayTransform: (value: Uint8Array) => T[I];
}
export type PacketField<T extends {}, I extends keyof T> =
  | PacketIntField<T, I>
  | PacketBytesField<T, I>;
