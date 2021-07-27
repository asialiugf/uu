(function(global, factory) {
  typeof exports === 'object' && typeof module !== 'undefined' ? module.exports = factory() :
    typeof define === 'function' && define.amd ? define(factory) :
    (global.barCharts = factory()); // 为 this 返回一个 barCharts。
}(this, (function() {
  function aa(x) {
    this.m = x
    alert("mmm")

    function aa1() {
      alert("aaaaa1")
    }
	this.aa2 = function() {
	   alert(this.m)
	   alert("this.aa1() aaaaa1")
	}

	alert( "befor" );
	aa1();
	this.aa2();
	alert( "after" );

  }
  alert ("88888888888888")
  let hh = new aa(100000)
  alert ("99999999999999")
  return aa;
})))
