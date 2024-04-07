import { readFileSync } from 'fs';
const wasm_module = new WebAssembly.Instance(new WebAssembly.Module(readFileSync("t1-c/a.out.wasm")), info);
var bocchiShutUp = wasm_module.exports.bocchiShutUp;
export { bocchiShutUp }