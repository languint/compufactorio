import { type Signal } from "./signals";

export function vsignal(
  signal: Signal,
  initialValue: number
) {
  console.log(signal.toString());
}
