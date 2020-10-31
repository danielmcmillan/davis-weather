import { IntFieldFormat, PacketDefinition } from "../types/PacketDefinition";
import { BarTrend, LoopPacketType } from "./common";
import {
  binStormDateToTimestamp,
  farenheitToCelsius,
  hundredthsOfHourPlusMinutesToMinutes,
  hundredthsOfInchToMm,
  milesToKilometres,
  noop,
  tenthsOfFarenheitToCelsius,
  tenthsOfMileToKilometres,
  thousandthsOfInHgToHPa,
} from "./conversions";

export interface Loop2Parsed {
  barTrend: BarTrend;
  packetType: LoopPacketType.LOOP2;
  barometer: number | null; // hPa
  insideTemperature: number | null; // degrees C
  insideHumidity: number | null;
  outsideTemperature: number | null; // degrees C
  windSpeed: number | null; // km/h
  windDirection: number | null; // degrees
  avgWindSpeed10Min: number | null; // km/h
  avgWindSpeed2Min: number | null; // km/h
  windGust10Min: number | null; // km/h
  windDirection10MinGust: number | null; // degrees
  dewPoint: number | null; // degrees C
  outsideHumidity: number | null;
  heatIndex: number | null; // degrees C
  windChill: number | null; // degrees C
  thswIndex: number | null; // degrees C
  rainRate: number | null; // mm/h
  uv: number | null;
  solarRadiation: number | null; // W/m^2
  stormRain: number | null; // mm
  stormStartDate: number | null; // ms since epoch
  dayRain: number | null; // mm
  last15MinuteRain: number | null; // mm
  lastHourRain: number | null; // mm
  dayET: number | null; // mm;
  last24HourRain: number | null; // mm
}

export const loop2Definition: PacketDefinition<Loop2Parsed> = {
  fields: [
    {
      id: "barTrend",
      offset: 3,
      intFormat: IntFieldFormat.Int8,
      intTransform: (value): BarTrend =>
        Object.values(BarTrend).includes(value) ? value : null,
    },
    {
      id: "packetType",
      offset: 4,
      intFormat: IntFieldFormat.UInt8,
      intTransform: noop,
    },
    {
      id: "barometer",
      offset: 7,
      intFormat: IntFieldFormat.UInt16LE,
      intTransform: thousandthsOfInHgToHPa,
    },
    {
      id: "insideTemperature",
      offset: 9,
      intFormat: IntFieldFormat.Int16LE,
      intTransform: tenthsOfFarenheitToCelsius,
    },
    {
      id: "insideHumidity",
      offset: 11,
      intFormat: IntFieldFormat.UInt8,
      intTransform: noop,
    },
    {
      id: "outsideTemperature",
      offset: 12,
      intFormat: IntFieldFormat.Int16LE,
      intTransform: tenthsOfFarenheitToCelsius,
    },
    {
      id: "windSpeed",
      offset: 14,
      intFormat: IntFieldFormat.UInt8,
      intTransform: milesToKilometres,
      isUnavailable: (data) => data[16] === 0 && data[17] === 0,
    },
    {
      id: "windDirection",
      offset: 16,
      intFormat: IntFieldFormat.UInt16LE,
      intTransform: noop,
      isUnavailable: (data) => data[16] === 0 && data[17] === 0,
    },
    {
      id: "avgWindSpeed10Min",
      offset: 18,
      intFormat: IntFieldFormat.UInt16LE,
      intTransform: tenthsOfMileToKilometres,
      isUnavailable: (data) => data[16] === 0 && data[17] === 0,
    },
    {
      id: "avgWindSpeed2Min",
      offset: 20,
      intFormat: IntFieldFormat.UInt16LE,
      intTransform: tenthsOfMileToKilometres,
      isUnavailable: (data) => data[16] === 0 && data[17] === 0,
    },
    {
      id: "windGust10Min",
      offset: 22,
      intFormat: IntFieldFormat.UInt16LE,
      intTransform: tenthsOfMileToKilometres,
      isUnavailable: (data) => data[16] === 0 && data[17] === 0,
    },
    {
      id: "windDirection10MinGust",
      offset: 24,
      intFormat: IntFieldFormat.UInt16LE,
      intTransform: noop,
      isUnavailable: (data) => data[16] === 0 && data[17] === 0,
    },
    {
      id: "dewPoint",
      offset: 30,
      intFormat: IntFieldFormat.Int16LE,
      intTransform: tenthsOfFarenheitToCelsius,
    },
    {
      id: "outsideHumidity",
      offset: 33,
      intFormat: IntFieldFormat.UInt8,
      intTransform: noop,
    },
    {
      id: "heatIndex",
      offset: 35,
      intFormat: IntFieldFormat.Int16LE,
      intTransform: farenheitToCelsius,
    },
    {
      id: "windChill",
      offset: 37,
      intFormat: IntFieldFormat.Int16LE,
      intTransform: farenheitToCelsius,
    },
    {
      id: "thswIndex",
      offset: 39,
      intFormat: IntFieldFormat.Int16LE,
      intTransform: farenheitToCelsius,
    },
    {
      id: "rainRate",
      offset: 41,
      intFormat: IntFieldFormat.UInt16LE,
      intTransform: hundredthsOfInchToMm,
    },
    {
      id: "uv",
      offset: 43,
      intFormat: IntFieldFormat.UInt8,
      intTransform: noop,
      isUnavailable: (data) => data[43] === 0xff,
    },
    {
      id: "solarRadiation",
      offset: 44,
      intFormat: IntFieldFormat.UInt16LE,
      intTransform: noop,
      isUnavailable: (data) => data[44] === 0xff,
    },
    {
      id: "stormRain",
      offset: 46,
      intFormat: IntFieldFormat.UInt16LE,
      intTransform: hundredthsOfInchToMm,
    },
    {
      id: "stormStartDate",
      offset: 48,
      intFormat: IntFieldFormat.UInt16LE,
      intTransform: binStormDateToTimestamp,
      isUnavailable: (data) => data[48] === 0xff && data[49] === 0xff,
    },
    {
      id: "dayRain",
      offset: 50,
      intFormat: IntFieldFormat.UInt16LE,
      intTransform: hundredthsOfInchToMm,
    },
    {
      id: "last15MinuteRain",
      offset: 52,
      intFormat: IntFieldFormat.UInt16LE,
      intTransform: hundredthsOfInchToMm,
    },
    {
      id: "lastHourRain",
      offset: 54,
      intFormat: IntFieldFormat.UInt16LE,
      intTransform: hundredthsOfInchToMm,
    },
    {
      id: "dayET",
      offset: 56,
      intFormat: IntFieldFormat.UInt16LE,
      intTransform: hundredthsOfInchToMm,
    },
    {
      id: "last24HourRain",
      offset: 58,
      intFormat: IntFieldFormat.UInt16LE,
      intTransform: hundredthsOfInchToMm,
    },
  ],
};
