import { IntFieldFormat, PacketDefinition } from "./types/PacketDefinition";

// Map value of IntFieldFormat to the function of Buffer used to read the int.
const readIntBufferFunctions: Record<
  IntFieldFormat,
  | "readInt8"
  | "readUInt8"
  | "readInt16LE"
  | "readInt16BE"
  | "readUInt16LE"
  | "readUInt16BE"
> = {
  [IntFieldFormat.Int8]: "readInt8",
  [IntFieldFormat.UInt8]: "readUInt8",
  [IntFieldFormat.Int16LE]: "readInt16LE",
  [IntFieldFormat.Int16BE]: "readInt16BE",
  [IntFieldFormat.UInt16LE]: "readUInt16LE",
  [IntFieldFormat.UInt16BE]: "readUInt16BE",
};

/**
 * Parse a packet of data based on a given packet definition.
 *
 * @param data The packet data as a Buffer.
 * @param packetDefinition Definition of the fields available in the data packet.
 * @returns An object containing the field values parsed from the data packet.
 */
export function parsePacket<T extends {}>(
  data: Buffer,
  packetDefinition: PacketDefinition<T>
): T {
  const result: T = ({} as unknown) as T;
  for (const fieldDefinition of packetDefinition.fields) {
    if (fieldDefinition.isUnavailable && fieldDefinition.isUnavailable(data)) {
      result[fieldDefinition.id] = (null as unknown) as T[keyof T];
    } else if ("intFormat" in fieldDefinition) {
      const rawValue = data[readIntBufferFunctions[fieldDefinition.intFormat]](
        fieldDefinition.offset
      );
      result[fieldDefinition.id] = fieldDefinition.intTransform(rawValue);
    } else {
      const rawValue = data.slice(
        fieldDefinition.offset,
        fieldDefinition.offset + fieldDefinition.byteArrayLength
      );
      result[fieldDefinition.id] = fieldDefinition.byteArrayTransform(rawValue);
    }
  }

  return result;
}
