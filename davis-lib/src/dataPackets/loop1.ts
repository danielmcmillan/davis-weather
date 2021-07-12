import { IntFieldFormat, PacketDefinition } from "../types/PacketDefinition";
import { BarTrend, LoopPacketType } from "./common";
import {
  binStormDateToTimestamp,
  hundredthsOfHourPlusMinutesToMinutes,
  hundredthsOfInchToMm,
  milesToKilometres,
  noop,
  thousandthsOfInHgToHPa,
} from "./conversions";

export interface InsideAlarms {
  fallingBarTrend: boolean;
  risingBarTrend: boolean;
  lowTemp: boolean;
  highTemp: boolean;
  lowHumidity: boolean;
  highHumidity: boolean;
  time: boolean;
}
export interface RainAlarms {
  highRainRate: boolean;
  rain15Minutes: boolean;
  rain24Hours: boolean;
  stormTotalRain: boolean;
  dailyET: boolean;
}
export interface OutsideAlarms {
  lowTemp: boolean;
  highTemp: boolean;
  windSpeed: boolean;
  windSpeed10MinAvg: boolean;
  lowDewpoint: boolean;
  highDewpoint: boolean;
  highHeat: boolean;
  lowWindChill: boolean;
  highTHSW: boolean;
  highSolarRadiation: boolean;
  highUV: boolean;
  uvDose: boolean;
  uvDoseEnabled: boolean;
  lowHumidity: boolean;
  highHumidity: boolean;
}
export interface ForecastIcons {
  rain: boolean;
  cloud: boolean;
  partlyCloudy: boolean;
  sun: boolean;
  snow: boolean;
}

export interface Loop1Parsed {
  barTrend: BarTrend;
  packetType: LoopPacketType.LOOP1;
  barometer: number | null; // hPa
  insideTemperature: number | null; // degrees C
  insideHumidity: number | null;
  outsideTemperature: number | null; // degrees C
  windSpeed: number | null; // km/h
  avgWindSpeed10Min: number | null; // km/h
  windDirection: number | null; // degrees
  outsideHumidity: number | null;
  rainRate: number | null; // mm/h
  uv: number | null;
  solarRadiation: number | null; // W/m^2
  stormRain: number | null; // mm
  stormStartDate: number | null; // ms since epoch for 12am local time of the start date
  dayRain: number | null; // mm
  monthRain: number | null; // mm
  yearRain: number | null; // mm
  dayET: number | null; // mm
  monthET: number | null; // mm
  yearET: number | null; // mm
  insideAlarms: InsideAlarms;
  rainAlarms: RainAlarms;
  outsideAlarms: OutsideAlarms;
  transmitterBatteryStatus: boolean; // true=bad
  consoleBatteryVoltage: number | null; // V
  forecastIcons: ForecastIcons;
  timeOfSunrise: number; // minutes after 12am local time
  timeOfSunset: number; // minutes after 12am local time
}

export const loop1Definition: PacketDefinition<Loop1Parsed> = {
  fields: [
    {
      id: "barTrend",
      offset: 3,
      intFormat: IntFieldFormat.Int8,
      intTransform: (value): BarTrend | null =>
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
      intTransform: hundredthsOfInchToMm,
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
      intTransform: hundredthsOfInchToMm,
    },
    {
      id: "windSpeed",
      offset: 14,
      intFormat: IntFieldFormat.UInt8,
      intTransform: milesToKilometres,
    },
    {
      id: "avgWindSpeed10Min",
      offset: 15,
      intFormat: IntFieldFormat.UInt8,
      intTransform: milesToKilometres,
    },
    {
      id: "windDirection",
      offset: 16,
      intFormat: IntFieldFormat.UInt16LE,
      intTransform: noop,
    },
    {
      id: "outsideHumidity",
      offset: 33,
      intFormat: IntFieldFormat.UInt8,
      intTransform: noop,
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
    },
    {
      id: "solarRadiation",
      offset: 44,
      intFormat: IntFieldFormat.UInt16LE,
      intTransform: noop,
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
      isUnavailable: (data) => data[46] === 0xff && data[47] === 0xff,
    },
    {
      id: "dayRain",
      offset: 50,
      intFormat: IntFieldFormat.UInt16LE,
      intTransform: hundredthsOfInchToMm,
    },
    {
      id: "monthRain",
      offset: 52,
      intFormat: IntFieldFormat.UInt16LE,
      intTransform: hundredthsOfInchToMm,
    },
    {
      id: "yearRain",
      offset: 54,
      intFormat: IntFieldFormat.UInt16LE,
      intTransform: hundredthsOfInchToMm,
    },
    {
      id: "dayET",
      offset: 56,
      intFormat: IntFieldFormat.UInt16LE,
      intTransform: (value) => hundredthsOfInchToMm(value / 10),
    },
    {
      id: "monthET",
      offset: 58,
      intFormat: IntFieldFormat.UInt16LE,
      intTransform: hundredthsOfInchToMm,
    },
    {
      id: "yearET",
      offset: 60,
      intFormat: IntFieldFormat.UInt16LE,
      intTransform: hundredthsOfInchToMm,
    },
    {
      id: "insideAlarms",
      offset: 70,
      byteArrayLength: 1,
      byteArrayTransform: ([value]) => ({
        fallingBarTrend: (value & 0x01) > 0,
        risingBarTrend: (value & 0x02) > 0,
        lowTemp: (value & 0x04) > 0,
        highTemp: (value & 0x08) > 0,
        lowHumidity: (value & 0x10) > 0,
        highHumidity: (value & 0x20) > 0,
        time: (value & 0x40) > 0,
      }),
    },
    {
      id: "rainAlarms",
      offset: 71,
      byteArrayLength: 1,
      byteArrayTransform: ([value]) => ({
        highRainRate: (value & 0x01) > 0,
        rain15Minutes: (value & 0x02) > 0,
        rain24Hours: (value & 0x04) > 0,
        stormTotalRain: (value & 0x08) > 0,
        dailyET: (value & 0x10) > 0,
      }),
    },
    {
      id: "outsideAlarms",
      offset: 72,
      byteArrayLength: 3,
      byteArrayTransform: ([value1, value2, value3]) => ({
        lowTemp: (value1 & 0x01) > 0,
        highTemp: (value1 & 0x02) > 0,
        windSpeed: (value1 & 0x04) > 0,
        windSpeed10MinAvg: (value1 & 0x08) > 0,
        lowDewpoint: (value1 & 0x10) > 0,
        highDewpoint: (value1 & 0x20) > 0,
        highHeat: (value1 & 0x40) > 0,
        lowWindChill: (value1 & 0x80) > 0,
        highTHSW: (value2 & 0x01) > 0,
        highSolarRadiation: (value2 & 0x02) > 0,
        highUV: (value2 & 0x04) > 0,
        uvDose: (value2 & 0x08) > 0,
        uvDoseEnabled: (value2 & 0x10) > 0,
        lowHumidity: (value3 & 0x04) > 0,
        highHumidity: (value3 & 0x08) > 0,
      }),
    },
    {
      id: "transmitterBatteryStatus",
      offset: 86,
      intFormat: IntFieldFormat.UInt8,
      intTransform: (value) => value !== 0,
    },
    {
      id: "consoleBatteryVoltage",
      offset: 87,
      intFormat: IntFieldFormat.UInt16LE,
      intTransform: (value) => (value * 300) / 51200,
    },
    {
      id: "forecastIcons",
      offset: 89,
      byteArrayLength: 1,
      byteArrayTransform: ([value]) => ({
        rain: (value & 0x01) > 0,
        cloud: (value & 0x02) > 0,
        partlyCloudy: (value & 0x04) > 0,
        sun: (value & 0x08) > 0,
        snow: (value & 0x10) > 0,
      }),
    },
    {
      id: "timeOfSunrise",
      offset: 91,
      intFormat: IntFieldFormat.UInt16LE,
      intTransform: hundredthsOfHourPlusMinutesToMinutes,
    },
    {
      id: "timeOfSunset",
      offset: 93,
      intFormat: IntFieldFormat.UInt16LE,
      intTransform: hundredthsOfHourPlusMinutesToMinutes,
    },
  ],
};
