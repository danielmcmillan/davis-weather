import { loop2Definition } from "./loopPackets/loop2";
import { mockLoop2 } from "./mock/loop2";
import { parsePacket } from "./parsePacket";

describe("parsePacket", () => {
  it("should parse LOOP2 packet", () => {
    const result = parsePacket(mockLoop2, loop2Definition);
    expect(result).toEqual({
      avgWindSpeed10Min: 0,
      avgWindSpeed2Min: 0,
      barTrend: 0,
      barometer: 1012.2108400000001,
      dayET: 0,
      dayRain: 0,
      dewPoint: -15.722222222222221,
      heatIndex: 11.11111111111111,
      insideHumidity: 54,
      insideTemperature: 23.111111111111107,
      last15MinuteRain: 0,
      last24HourRain: 0,
      lastHourRain: 0,
      outsideHumidity: 54,
      outsideTemperature: 11.722222222222221,
      packetType: 1,
      rainRate: 0,
      solarRadiation: null,
      stormRain: 0,
      stormStartDate: null,
      thswIndex: 123.88888888888889,
      uv: null,
      windChill: 11.666666666666666,
      windDirection: 306,
      windDirection10MinGust: 315,
      windGust10Min: 0,
      windSpeed: 0,
    });
  });
});
