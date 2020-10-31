export const noop = <T>(value: T): T => value;
export const thousandthsOfInHgToHPa = (value: number) => value * 0.03386;
export const tenthsOfFarenheitToCelsius = (value: number) => ((value / 10 - 32) * 5) / 9;
export const farenheitToCelsius = (value: number) =>
  tenthsOfFarenheitToCelsius(value * 10);
export const milesToKilometres = (value: number) => value * 1.60934;
export const tenthsOfMileToKilometres = (value: number) => milesToKilometres(value / 10);
export const hundredthsOfInchToMm = (value: number) => value * 25.4 * 0.01;
export const binStormDateToTimestamp = (value: number) =>
  new Date(value & (0x007f + 2000), value & (0xf000 - 1), value & (0x0f80 - 1)).getTime();
export const hundredthsOfHourPlusMinutesToMinutes = (value: number) =>
  Math.floor(value / 100) * 60 + (value % 100);
