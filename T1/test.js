import assert from "assert"

// 失败的导入1：
// const { Module } = pkg;
// import pkg from "./t1_cpp/a.out.cjs";

// 失败的导入2：
// var bocchiShutUp = Module._bocchiShutUp();

// 失败的导入3：
// const { _bocchiShutUp: bocchiShutUp} = pkg;
// import pkg from "./t1_cpp/a.out.cjs"



// Choose proper "import" depending on your PL.
// import { bocchiShutUp } from "./t1-as/build/release.js";
// import { bocchi_shut_up as bocchiShutUp } from "./t1_rust/pkg/t1_rust.js"
// import { bocchi_shut_up as bocchiShutUp } from "./t1_cpp/a.out.cjs"
// [Write your own "import" for other PLs.]

assert.strictEqual(bocchiShutUp(1,[13,14,15,21,11,16],6),10);
assert.strictEqual(bocchiShutUp(2,[13,14,13,11,21,13,21,22],8),21);

console.log("🎉 You have passed all the tests provided.");
