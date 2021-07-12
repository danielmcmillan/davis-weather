import { IntFieldFormat, PacketDefinition, PacketField } from "../types/PacketDefinition";
import { loop2Definition, Loop2Parsed } from "./loop2";

export interface Loop2WithVoltage extends Loop2Parsed {
  usbVoltage: number | null;
  batteryVoltage: number | null;
}

export const loop2WithVoltageDefinition: PacketDefinition<Loop2WithVoltage> = {
  fields: [
    ...(loop2Definition.fields as PacketField<
      Loop2WithVoltage,
      keyof Loop2WithVoltage
    >[]),
    {
      id: "usbVoltage",
      offset: 99,
      intFormat: IntFieldFormat.UInt16BE,
      // 2932 = 5.0v
      intTransform: (value): number => (value * 5.0) / 2932,
      isUnavailable: (data) => data.byteLength < 101,
    },
    {
      id: "batteryVoltage",
      offset: 101,
      intFormat: IntFieldFormat.UInt16BE,
      // 3322 = 4.1v
      intTransform: (value): number => (value * 4.1) / 3322,
      isUnavailable: (data) => data.byteLength < 101,
    },
  ],
};
