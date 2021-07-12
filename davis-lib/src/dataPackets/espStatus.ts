import { IntFieldFormat, PacketDefinition } from "../types/PacketDefinition";

export interface DavisStatusParsed {
  usbVoltage: number | null;
  batteryVoltage: number | null;
}

export const davisStatusDefinition: PacketDefinition<DavisStatusParsed> = {
  fields: [
    {
      id: "usbVoltage",
      offset: 0,
      intFormat: IntFieldFormat.UInt16BE,
      // 2932 = 5.0v
      intTransform: (value): number => (value * 5.0) / 2932,
    },
    {
      id: "batteryVoltage",
      offset: 2,
      intFormat: IntFieldFormat.UInt16BE,
      // 3322 = 4.1v
      intTransform: (value): number => (value * 4.1) / 3322,
    },
  ],
};
