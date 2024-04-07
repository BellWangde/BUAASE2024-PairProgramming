import fs from "fs"

export function mancalaOperator(n, arr) {
  const binary = fs.readFileSync('./t3-2-c/a.out.wasm');

  var HEAPU8 = new Uint8Array(4294967296);
  var _emscripten_resize_heap = (requestedSize) => {
    var oldSize = HEAPU8.length;
    // With CAN_ADDRESS_2GB or MEMORY64, pointers are already unsigned.
    requestedSize >>>= 0;
    // abortOnCannotGrowMemory(requestedSize);
  };

  var wasmImports = {
    emscripten_resize_heap: _emscripten_resize_heap
  };

  var info = {
    'env': wasmImports,
    'wasi_snapshot_preview1': wasmImports,
  };

  const module = new WebAssembly.Module(binary);
  const instance = new WebAssembly.Instance(module, info);

  const memory = instance.exports.memory;
  const ptr = instance.exports.stackAlloc(14 * 4);
  const start = ptr >> 2;
  const heap = new Int32Array(memory.buffer);
  for (let i = 0; i < 14; ++ i) {
    heap[start + i] = arr[i];
  }
  return instance.exports.mancalaOperator(n, ptr);
}

// console.log(mancalaOperator(1, [4,4,4,4,0,5,1,0,6,5,5,5,5,0,1]));