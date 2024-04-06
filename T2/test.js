import assert from "assert";

import {mancalaResult} from "./t2_cpp/a.out.js";
// Choose proper "import" depending on your PL.
// import { mancalaResult } from "./t2-as/build/release.js";
// import { mancala_result as mancalaResult } from "./t2_rust/pkg/t1_rust.js"
// [Write your own "import" for other PLs.]

assert.strictEqual(mancalaResult(1,[11,12],2),30001);
assert.strictEqual(mancalaResult(1,[14],1),20001);
assert.strictEqual(mancalaResult(1, [16,22],2), 20001);
assert.strictEqual(mancalaResult(1, [16,23,12],3), 20007);
assert.strictEqual(mancalaResult(2, [23,13], 2), 30001);
assert.strictEqual(mancalaResult(1, [13,16], 2), 20002);
// assert.strictEqual(mancalaResult(1, [13,16,22,21,12,23,11,24,12,22,11,26], 12),15012);
assert.strictEqual(mancalaResult(1, [13,16,25,11,22,24,12,21,11,22,16,15,21,16,14,22,16,15],18), 14990);

console.log("🎉 You have passed all the tests provided.");
