import assert from "assert"
import { bocchiShutUp } from './t1_cpp/a.out.js';
//
// let mod = require("./t1_cpp/a.out.js")
// let bocchiShutUp = mod.cwrap("bocchiShutUp", "number", ["number", "array", "number"])
// import { cwrap } from "./t1_cpp/a.out.js"

// var bocchiShutUp = Module.cwrap("bocchiShutUp", "number", ["number", "array", "number"])
// console.log(bocchiShutUp(1,1,6));
assert.strictEqual(bocchiShutUp(1,[13,14,15,21,11,16],6),10);
assert.strictEqual(bocchiShutUp(2,[13,14,13,11,21,13,21,22],8),21);

console.log("🎉 You have passed all the tests provided.");
