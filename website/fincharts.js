/* charmi liu version 1.0 */
/*
 (  function(global,factory){} ( this,(fuction(){}) )    );
 */
(function(global, factory) {
  typeof exports === 'object' && typeof module !== 'undefined' ? module.exports = factory() :
    typeof define === 'function' && define.amd ? define(factory) :
    (global.barCharts = factory());
}(this, (function() {
  'use strict';

  let SERIES_TYPE = [
    'bar',
    'candlestick',
    'line'
  ];

  /* ------------------------------------------- data defined here ---------------------------------- */
  /*
	下面的 let option = {}
	是一个option的例子，在 index.html 中 let op =  new barCharts();之后,会用到
	op.update(option),就是 this.update(opt)的参数。

	其中要注意的是 大Y，是整个Y轴数据，Y.data[0]... Y.data[n]，data数组，表示有多个显示块
	可以分为 main, sub 两种类型，其中 main 块，主要显示K柱，及相应的指标， sub主要显示副
	图指标，比如显示：MACD，KDJ等。可以有多个sub类型。

	每个Y.data[0].y，就是每个data的小y,也会有多个数据，比如 主图中的 K柱就要有 open
	close highest lowest数据，最少四组。

	要注意： 在javascript中，数组的let oo = []; let xx = oo ; 这里的 oo 和 xx 是同一组数据，
	如果更改了oo，则xx 也被更改了。 所以要注意，小y里面有多组数据，其中有几组数据的来源相
	同的话，注意在程序中要小心，只需要更新一个即可。

	另外，this.update的输入的option，只需要将更新的数据放在option中，不需要所有的数据。
	比如，初始化时，输入了1000个K柱数据，每次新来一个K柱的数据，则不需要在option 放入1001个数
	据，只需要放最后一个K柱数据即可。

	还要注意： 不管main, sub，所有的数据来源，其长度必须一致。比如 open close ... 包括
	MACD的每条线，当前显示第 1001，下一个显示 第1002，所有的数据当前必须是1001长，下一个必须
	是1002长。即 open.length 和 close.length 还有 sub里面的所有的数据长度，都必须一样长。

	this.update()参数 option.type有三种类型：0，1，2。在后端从服务器送过来的的option数据
	必须设定type值。具体参看 this.update() 的说明。

	关于数据的说明：

	1、下面的option 中的数据，将会一直保存。
	2、由于javascript 数组赋值时，其数据只是一份，也就是公用原型数据，因此 需要特别注意数据的修改。
	   比如：
	   let aa = [];
	   let bb = aa;
	   如果修改了 aa，则 bb 也变了。
	   */
  let opt_oo = [];
  let opt_hh = [];
  let opt_ll = []; //
  let opt_cc = [];
  let opt_tt = []; // X轴的时间，是字串类型

  let MAIN = {
    title: '中国联通',
    type: 'candle',
    position: 'main',
    kedu: [],
    x: 2,
    y: [{
      type: 'o',
      data: opt_oo
    }, {
      type: 'h',
      data: opt_hh
    }, {
      type: 'l',
      data: opt_ll
    }, {
      type: 'c',
      data: opt_cc
    }, {
      title: 'MA5',
      type: 'line',
      data: []
    }]
  };

  let MACD = {
    title: 'MACD',
    position: 'sub',
    y: [{
      title: 'diff',
      type: 'line',
      data: opt_oo
    }, {
      title: 'dea',
      type: 'line',
      data: opt_cc
    }]
  };

  let VOL = {
    title: 'Vol',
    position: 'sub',
    kedu: [],
    x: 3,
    y: [{
      type: 'bar',
      data: []
    }, {
      type: 'line',
      data: [] // ma5
    }]
  };

  let KDJ = {

  };

  let RSI = {

  };

  let option = {
    type: 0,
    X: {
      data: [{
        x: opt_tt
      }, {
        x: opt_tt
      }]
    },
    Y: {
      data: [MAIN, VOL, MACD, KDJ]
    }
  };
  //option.Y.data[0].y[0].data
  /* ------------------------------------------- data defined here ---------------------------------- */

  option.Y.data.push(MACD);

  function stockcharts(opt, theme) {

    let R = Math.round; //3.5 将舍入为 4，而 -3.5 将舍入为 -3。
    let F = Math.floor; //1.8 ==>1
    let C = Math.ceil; //1.4 ==>2
    let P = []; //记录屏幕px位置对应的K数组下标
    let X, Y;

    /* define G global var !!!! ------------------------------------------------------- */
    let tWid = window.devicePixelRatio;
    let tW = window.innerWidth > 400 ? window.innerWidth : 400, //total width
      H1 = window.innerHeight * 0.60 > 350 ? R(window.innerHeight * 0.60 / 10) * 10 : 350, // candle window height
      H2 = window.innerHeight * 0.35 > 200 ? R(window.innerHeight * 0.35 / 10) * 10 : 200; // MACD KDJ window height
    //tW = tW / tWid;
    H1 = H1 / tWid;
    H2 = H2 / tWid;
    let H3 = R(H2 * 0.2);
    let topW = tW,
      botW = tW,
      topH = 0 / tWid,
      botH = 30 / tWid,
      leftW = 60 / tWid,
      rightW = 200 / tWid,
      scrollFH = 5 / tWid,
      scrollBH = 5 / tWid,

      mainFW = tW - rightW - leftW,
      mainFH = H1,
      mainBW = tW - rightW,
      mainBH = H1,

      scrollFW = mainFW,
      scrollBW = mainBW,

      subFW = mainFW,
      subFH = H2,
      subBW = mainBW,
      subBH = H2,

      sttFW = mainFW,
      sttFH = mainFH,
      sttBW = mainBW,
      sttBH = mainBH,

      backFW = mainFW,
      backFH = H1 + topH + scrollFH + H2 * 3 + botH,
      backBW = mainBW,
      backBH = backFH,

      leftH = backFH - topH - botH,
      rightH = backFH - topH - botH;


    let G = {
      AB: [], // levelInit()
      PP: [], // 记录用于显示的每个K'柱的起始K，结束K，以及显示于屏幕的起始位置X
      XX: [], // 记录屏幕X位置上显示的K柱index。
      _winX: 0,
      _winY: 0,
      _maxLen: 20000, //  max data len
      _datLen: 0,
      _disLen: 8000, //  max display len
      _disLast: 1, //  display the most right bar !
      _barW: 0,
      _halfBarW: 0,
      _barB: 0,
      _barE: 0,
      _minLevel: 0, // min display Level
      _curLevel: 0, // current display Level
      _maxLevel: 0 // max display Level
    };

    let cD = []; // canvas Display !!
    let frame = []; // canvas Display !!
    let cvsConf = {
      topNav: {
        backgroundColor: "#111111",
        zIndex: 105,
        border: 0,
        top: 0,
        left: 0,
        width: topW,
        height: topH
      },
      leftNav: {
        backgroundColor: "#222222",
        zIndex: 105,
        top: topH,
        left: 0,
        width: leftW,
        height: leftH
      },
      rightNav: {
        backgroundColor: "#222222",
        zIndex: 105,
        top: topH,
        left: tW - rightW,
        width: rightW,
        height: 100
        //height: rightH
      },
      botNav: {
        backgroundColor: "#111111",
        zIndex: 105,
        top: backFH - botH,
        left: 0,
        width: botW,
        height: botH
      },
      mainB: {
        backgroundColor: "#000000",
        //backgroundColor: "#776633",
        zIndex: 99,
        top: topH,
        left: 0,
        width: mainBW,
        height: mainBH
      },
      mainF: {
        backgroundColor: "transparent",
        //backgroundColor: "#776633",
        zIndex: 101,
        top: topH,
        left: leftW,
        width: mainFW,
        height: mainFH
      },
      scrollB: {
        backgroundColor: "#229999",
        zIndex: 104,
        top: topH + mainFH,
        left: 0,
        width: scrollBW,
        height: scrollBH
      },
      scrollF: {
        backgroundColor: "#229999",
        zIndex: 105,
        top: topH + mainFH,
        left: leftW,
        width: scrollFW,
        height: scrollFH
      },
      subS: [],
      subB: {
        backgroundColor: "transparent",
        zIndex: 99,
        top: topH + H1 + scrollFH,
        left: 0,
        width: subBW,
        height: subBH
      },
      subF: {
        backgroundColor: "transparent",
        zIndex: 101,
        top: topH + H1 + scrollFH,
        left: leftW,
        width: subFW,
        height: subFH
      },
      /*
	  sub1B: {
		backgroundColor: "transparent",
		zIndex: 94,
		top: topH + H1 + scrollFH,
		left: 0,
		width: subBW,
		height: subBH
	  },
	  sub1F: {
		backgroundColor: "transparent",
		zIndex: 95,
		top: topH + H1 + scrollFH,
		left: leftW,
		width: subFW,
		height: subFH
	  },
	  sub2B: {
		backgroundColor: "transparent",
		zIndex: 12,
		top: topH + H1 + scrollFH + H2,
		left: 0,
		width: subBW,
		height: subBH
	  },
	  sub2F: {
		backgroundColor: "transparent",
		zIndex: 12,
		top: topH + H1 + scrollFH + H2,
		left: leftW,
		width: subFW,
		height: subFH
	  },
	  sub3B: {
		backgroundColor: "transparent",
		zIndex: 12,
		top: topH + H1 + scrollFH + H2 * 2,
		left: 0,
		width: subBW,
		height: subBH
	  },
	  sub3F: {
		backgroundColor: "transparent",
		zIndex: 12,
		top: topH + H1 + scrollFH + H2 * 2,
		left: leftW,
		width: subFW,
		height: subFH
	  },
	  */
      sttB: {
        backgroundColor: "#222222",
        zIndex: 99,
        top: topH,
        left: 0,
        width: sttBW,
        height: sttBH
      },
      sttF: {
        backgroundColor: "#222222",
        zIndex: 101,
        top: topH,
        left: leftW,
        width: sttFW,
        height: sttFH
      },
      backB: {
        backgroundColor: "#000000",
        zIndex: 49,
        top: 0,
        left: 0,
        width: backBW,
        height: backBH
      },
      backF: {
        backgroundColor: "transparent",
        zIndex: 100,
        top: 0,
        left: leftW,
        width: backFW,
        height: backFH
      }
    }

    /* G definProperties begin         --------------------------*/
    /* Object对象，是所有JavaScript对象的超类(基类)。Object.prototype(Obecjt的原型)定义了Js对象的基本方法和属性。 */
    Object.defineProperties(G, {
      "disLast": {
        set: function(x) {
          this._disLast = x;
        },
        get: function() {
          return this._disLast;
        },
        enumerable: true,
        configurable: true
      },
      "curLevel": {
        set: function(x) {
          setCurLevel(x);
        },
        get: function() {
          return this._curLevel;
        },
        enumerable: true,
        configurable: true
      },
      "minLevel": {
        set: function(x) {
          this._minLevel = x;
        },
        get: function() {
          return this._minLevel;
        },
        enumerable: true,
        configurable: true
      },
      "maxLevel": {
        set: function(x) {
          this._maxLevel = x;
        },
        get: function() {
          return this._maxLevel;
        },
        enumerable: true,
        configurable: true
      },
      "barW": {
        set: function(x) {
          this._barW = x;
        },
        get: function() {
          return this._barW;
        },
        enumerable: true,
        configurable: true
      },
      "halfBarW": {
        set: function(x) {
          this._halfBarW = x;
        },
        get: function() {
          return this._halfBarW;
        },
        enumerable: true,
        configurable: true
      },
      "barB": {
        set: function(x) {
          //this._barB = x;
          setBE(x);
        },
        get: function() {
          return this._barB;
        },
        enumerable: true,
        configurable: true
      },
      "barE": {
        set: function(x) {
          this._barE = x;
        },
        get: function() {
          return this._barE;
        },
        enumerable: true,
        configurable: true
      },
      "maxLen": {
        set: function(x) {
          this._maxLen = x;
        },
        get: function() {
          return this._maxLen;
        },
        enumerable: true,
        configurable: true
      },
      "datLen": {
        set: function(x) {
          this._datLen = x;
        },
        get: function() {
          return this._datLen;
        },
        enumerable: true,
        configurable: true
      },
      "disLen": {
        set: function(x) {
          this._disLen = x;
        },
        get: function() {
          return this._disLen;
        },
        enumerable: true,
        configurable: true
      }
    });

    /* set G._curLevel and modify Begin and End index */
    /*
	   function setCurLevel(x) 会在 G.curLevel = x 时，被调用，它会修改 G._barB,
	   G._barE的值。
	   */
    function setCurLevel(x) {
      if (x < 0) {
        G._curLevel = 0;
      } else if (x > G._maxLevel) {
        G._curLevel = G._maxLevel;
      } else {
        G._curLevel = x;
      }
      console.log(" G._curLevel " + G._curLevel);
      let dataLen = Math.min(G.AB[G._curLevel][2], G._disLen, G._datLen);
      if (dataLen == 0) {
        G._barB = 0;
        G._barE = 0;
        G._disLast = 1;
        G._barW = G.AB[G._curLevel][0] > 0 ? G.AB[G._curLevel][0] : 1;
        G._halfBarW = (G._barW - 1) / 2;
        return;
      }
      G._barW = G.AB[G._curLevel][0] > 0 ? G.AB[G._curLevel][0] : 1;
      G._halfBarW = (G._barW - 1) / 2;
      G._barE = R((dataLen + G._barB + G._barE) / 2);
      G._barB = G._barE - dataLen;
      if (G._barB < 0) {
        G._barB = 0;
        G._barE = dataLen;
      };
      if (G._barE >= G._datLen || G._disLast == 1) { //最后一个K柱
        G._barE = G._datLen;
        G._barB = G._datLen - dataLen;
      }
      G._disLast = G._barE == G._datLen ? 1 : 0;
      G._curLen = G._barE - G._barB;
    }
    /*
	   function setBE(m = 0) 在G.barB = 9; 给G.barB赋值时会调用，如果你想把显示的起始
	   位置向前提5个K柱，那可以这样： G.barB = G.barB -5 ;
	   */
    function setBE(m = 0) {
      let y;
      let x = m - G._barB;
      if (x > 0) {
        y = (G._barE + x) < G._datLen ? x : G._datLen - G._barE;
        G._barE = G._barE + y;
        G._barB = G._barB + y;
      } else if (x < 0) {
        y = m > 0 ? x : 0 - G._barB;
        G._barE = G._barE + y;
        G._barB = G._barB + y;
      }
      G._disLast = G._barE == G._datLen ? 1 : 0;
      /*
	  G._disLast表示当前是否显示最后一个K柱,最后一个K柱在实际中，可能会因为一直从服务
	  器上收到信息而增加。
	  上面 这一条语句，表示，如果当前显示到最后一个K柱（即 G._barE == G._datLen），则
	  以后一直会显示到最后。
	  其作用，主要用于：生产环境中，可能会静态观察中间某一段，看完后，又需要回到 一直
	  显示最后一个K柱的变化，G._disLast = 1,能保证 最后一个K柱的变化一直能被显示出来。
	  */
    }

    function levelInit() {
      let rate = 1.1;
      for (let i = 18; i >= 11; i--) {
        let a1 = 2 * i + 1;
        let sp = R(i / 5);
        G.AB.push([a1, sp, F(mainFW / (a1 + sp))]);
      }
      for (let i = 10; i >= 2; i--) {
        let a1 = 2 * i + 1;
        G.AB.push([a1, 2, F(mainFW / (a1 + 2))]); // 1px 空
        G.AB.push([a1, 1, F(mainFW / (a1 + 1))]);
      }
      let baseLen = mainFW / 5;
      for (let i = 0; i <= 5; i++) {
        G.AB.push([3, 0, F(baseLen)]);
        baseLen = baseLen * rate;
      }
      for (let i = 0; i <= 37; i++) {
        G.AB.push([1, 0, F(baseLen)]);
        baseLen = baseLen * rate;
      }
      G.curLevel = 0;
      G._maxLevel = G.AB.length - 1;
    }
    levelInit();
    /* define G end! ------------------------------------------------------- */


    /* canvas function begin !! ----------------------------------------------- */
    function createCvs(id, c = {}) {
      let canvas = document.createElement("canvas");
      document.body.appendChild(canvas);
      let ctx = canvas.getContext("2d");
      canvas.style.backgroundColor = c.backgroundColor;
      canvas.style.visibility = "visible";
      canvas.style.position = "absolute";
      //canvas.style.border = c.border;
      canvas.style.zIndex = c.zIndex;
      canvas.style.left = c.left;
      canvas.style.top = c.top;
      canvas.width = c.width;
      canvas.height = c.height;
      canvas.id = id;
      return [canvas, ctx, id];
    }

    function _topNav() {
      let c = new createCvs("topNav", cvsConf.topNav);
      this.cvs = c[0];
      this.ctx = c[1];
      //cvs[0].style.position = "fixed";
      this.ctx.fillStyle = "#AA0000";
      this.ctx.fillRect(0, topH - 1, topW, 1);
    }

    function _leftNav() {
      let cvs = new createCvs("leftNav", cvsConf.leftNav);
      this.cvs = cvs;
      //cvs[0].style.position = "fixed";
      let ctx = cvs[1];
      ctx.fillStyle = "#AA0000";
      ctx.fillRect(0, topH + 200, leftW, 1);
    }
    //let leftNav = new _leftNav();

    function _rightNav() {
      let c = new createCvs("rightNav", cvsConf.rightNav);
      this.cvs = c[0];
      this.ctx = c[1];
      //cvs[0].style.position = "fixed";
    }

    function _botNav() {
      let c = new createCvs("botNav", cvsConf.botNav);
      this.cvs = c[0];
      this.ctx = c[1];
    }

    // 主窗口，前端显示 candle，均线等
    function _mainF() {
      let c = new createCvs("mainF", cvsConf.mainF);
      this.cvs = c[0];
      this.ctx = c[1];
      this.id = c[2];
      this.mn = []; // 计算 Y轴刻度 [ m,n, hmax,lmin,top,down ]
      this.oo = [];
      this.hh = [];
      this.ll = [];
      this.cc = [];
      this.xx = []; //X轴的坐标 （时间）
      this.line = []; // 用于保存有多少条line需要画。
      this.bar = [];
      this.cvs.addEventListener("mousemove", mouseMove, false);
      //this.cvs.addEventListener("mouseleave", mouseMove, false);
      //cvs.addEventListener("mouseout", mouseMove, false);
      this.cvs.addEventListener('wheel', xxxx, false);

      this.cvs.addEventListener('dblclick', xx.bind(this), false);

      function xx(e) {
        this.drawCandle();
      }

      this.calcMN = function() {}
      this.clear = function() {
        this.ctx.clearRect(0, 0, this.cvs.width, this.cvs.height);
      }

      function calcPosition() { // 使用不了 this.xx 这种变量
        let position = 0;
        let posB = -1;
        let idxB = G.barB;
        G.PP = [];
        G.XX = [];
        for (let i = G.barB; i < G.barE; i++) {
          position = F(((i - G.barB) / G.AB[G.curLevel][2]) * c[0].width);
          if (position == posB && i != (G.barE - 1)) {
            continue;
          } else {
            G.PP.push([idxB, (i + 1), position]) // position 是K'柱的起始位置, position + G.halfBarW，是K'的中心位置
            for (let j = position; j < (position + G.barW); j++) {
              G.XX[j] = i;
            }
            posB = position;
            idxB = i + 1;
          }
        }
      }

      this.drawX = function() { //显示 x 轴？
        if (this.xx.length <= 0) {
          return;
        }
        this.ctx.fillRect(0, this.cvs.height - 12, this.cvs.width, 1);
        let space = C(G.AB[G.curLevel][2] / this.cvs.width * 80);
        for (let i = G.barB; i < G.barE; i = i + space) {
          let position = F(((i - G.barB) / G.AB[G.curLevel][2]) * this.cvs.width);
          this.ctx.fillRect(position + G.halfBarW, this.cvs.height - 15, 1, 5);
          this.ctx.fillText(this.xx[i], position, (this.cvs.height));
        }
      }

      /*
		drawCandle() 必须先调用之后，才会修改 this.mn 和 this.PP
		其它的显示会依赖 this.mn 和 this.PP
		*/
      this.drawCandle = function() { //采用this.aaa = function() 这种方式，内部可以使用this.xx变量。
        calcPosition();
        if (G.barB == G.barE) {
          return;
        }
        let oo = [],
          hh = [],
          ll = [],
          cc = [];
        let cvs = this.cvs;
        let ctx = this.ctx;

        for (let i = 0; i < G.PP.length; i++) { //G.PP.length ：屏幕上总共要显示的K柱数量
          /*
           i:表示第几个要显示的K柱，这个要显示的柱子其实有可能代表了多个实际bar，超始为 G.PP[i][0] 结束为 G.PP[i][1]
		   下面的 hhh,表示要显示的最高值，也就是 这多个实际bar的最高。同理 lll。
		   也就是相当于要把这些个实际的bars 合并成一个要显示的 K柱子。 
		  */
          let h1 = this.hh.slice(G.PP[i][0], G.PP[i][1]);
          let l1 = this.ll.slice(G.PP[i][0], G.PP[i][1]);
          let hhh = Math.max.apply(null, h1);
          let lll = Math.min.apply(null, l1);
          hh.push(hhh);
          ll.push(lll);
          oo.push(this.oo[G.PP[i][0]]);
          cc.push(this.cc[G.PP[i][1] - 1]);
        }

        let o, h, l, c, dif1, dif2;
        this.mn = calcYkedu0(this.cvs.height, hh, ll);
        for (let i = 0; i < G.PP.length; i++) {
          if (G.halfBarW != 0) {
            o = R(this.mn[0] * oo[i] + this.mn[1]);
            c = R(this.mn[0] * cc[i] + this.mn[1]);
            dif1 = c - o;
            if (dif1 == 0) {
              dif1 = 1;
            }
          }
          h = R(this.mn[0] * hh[i] + this.mn[1]);
          l = R(this.mn[0] * ll[i] + this.mn[1]);
          dif2 = l - h;
          if (dif2 == 0) {
            dif2 = 1;
          }
          if (cc[i] < oo[i]) {
            ctx.fillStyle = "#008888";
            if (G.halfBarW != 0) {
              ctx.fillRect(G.PP[i][2], o, G.barW, dif1);
              ctx.fillRect(G.PP[i][2] + G.halfBarW, h, 1, dif2);
            } else {
              ctx.fillRect(G.PP[i][2] + G.halfBarW, h, 1, dif2);
            }
          } else {
            ctx.fillStyle = "#880000";
            if (G.halfBarW != 0) {
              ctx.fillRect(G.PP[i][2], c, G.barW, 0 - dif1);
              ctx.fillRect(G.PP[i][2] + G.halfBarW, h, 1, dif2);
            } else {
              ctx.fillRect(G.PP[i][2] + G.halfBarW, h, 1, dif2);
            }
          }
        }
        this.drawX();
      } /* end drawCandle()  */

    } /* end _mainF */

    // 主窗口，后端，根据鼠标的移动，显示当前 K柱 的相关信息，包括 ohlc,均线值，增长百分比等
    function _mainB() {
      let c = new createCvs("mainB", cvsConf.mainB);
      this.cvs = c[0];
      this.ctx = c[1];
      this.kedu = [];
      this.mn = [];
      //cvs[0].style.position = "fixed";
      //c[0].addEventListener("mousemove", mouseMove, false);
      //c[0].addEventListener('wheel', xxxx, false);

      this.initScale = function() {
        c[1].fillStyle = "#773388";
        this.kedu = [];
        this.kedu.push([30, "100%"]);
        this.kedu.push([R(30 + (mainBH - 60) * 0.191), "19.1%"]);
        this.kedu.push([R(30 + (mainBH - 60) * 0.382), "38.2%"]);
        this.kedu.push([R(30 + (mainBH - 60) * 0.500), "50.0%"]);
        this.kedu.push([R(30 + (mainBH - 60) * 0.618), "61.8%"]);
        this.kedu.push([R(30 + (mainBH - 60) * 0.809), "80.9%"]);
        this.kedu.push([mainBH - 30, "00.0%"]);
      }

      this.dispScale = function() {
        c[1].fillStyle = "#773388";
        c[1].font = "14px";
        c[1].fillRect(leftW - 2, 0, 1, mainBH);
        for (let i = 0; i < this.kedu.length; i++) {
          c[1].fillStyle = "#773388";
          c[1].fillRect(leftW - 6, this.kedu[i][0], 4, 1);
          c[1].fillStyle = "#222222";
          c[1].fillRect(leftW - 1, this.kedu[i][0], mainBW - leftW, 1);
          c[1].fillStyle = "#AAAAAA";
          c[1].fillText(this.kedu[i][1], leftW - 38, this.kedu[i][0]);
        }
      }
      this.initScale();
      this.dispScale();

      /*
		dispYdata() :
		当鼠标移动时，Y轴显示鼠标Y所表示的股票的价格，显示在mainB的左边。
		其中，需要使用到mainF中的mn数据。
		*/
      this.dispYdata = function(y) {
        if (y < frame[0][0].mn[4] || y > (this.cvs.height - frame[0][0].mn[5])) {
          return;
        }
        let yy = R((y * (frame[0][0].mn[3] - frame[0][0].mn[2]) + frame[0][0].mn[2] *
            (this.cvs.height - frame[0][0].mn[5]) - frame[0][0].mn[3] * frame[0][0].mn[4]) /
          (this.cvs.height - frame[0][0].mn[5] - frame[0][0].mn[4]) * 100) / 100;
        this.ctx.fillStyle = "#770088";
        this.ctx.fillText(yy, 10, y);
      }

      this.clear = function() {
        this.ctx.clearRect(0, 0, this.cvs.width, this.cvs.height);
      }
    }

    function _scrollB() {
      let cvs = new createCvs("scrollB", cvsConf.scrollB);
      this.cvs = cvs;
      //cvs[0].style.position = "fixed";
    }

    function _scrollF() {
      let cvs = new createCvs("scrollF", cvsConf.scrollF);
      this.cvs = cvs;
      //cvs[0].style.position = "fixed";
    }

    function _subB() {
      let c = new createCvs("subB", cvsConf.subB);
      this.cvs = c[0];
      this.ctx = c[1];
      this.idx = 0;
      this.kedu = [];
      //cvs[0].style.position = "fixed";
      c[0].addEventListener("mousemove", mouseMove, false);
      //c[0].addEventListener('wheel', xxxx, false);
      //this.cvs.addEventListener('dblclick', frameChange(this.idx, 0), false);
      this.clear = function() {
        this.ctx.clearRect(0, 0, this.cvs.width, this.cvs.height);
      }

      this.init = function() {
        this.ctx.clearRect(0, 0, this.cvs.width, this.cvs.height);
        this.ctx.fillStyle = "#AA0000";
        this.ctx.fillRect(0, this.cvs.height - 1, subBW, 1);
        c[1].fillStyle = "#773388";
        this.kedu = [];
        this.kedu.push([10, "100%"]);
        this.kedu.push([R(10 + (this.cvs.height - 20) * 0.191), "19.1%"]);
        this.kedu.push([R(10 + (this.cvs.height - 20) * 0.382), "38.2%"]);
        this.kedu.push([R(10 + (this.cvs.height - 20) * 0.500), "50.0%"]);
        this.kedu.push([R(10 + (this.cvs.height - 20) * 0.618), "61.8%"]);
        this.kedu.push([R(10 + (this.cvs.height - 20) * 0.809), "80.9%"]);
        this.kedu.push([this.cvs.height - 10, "00.0%"]);

        c[1].fillStyle = "#773388";
        c[1].font = "14px";
        c[1].fillRect(leftW - 2, 0, 1, this.cvs.height);
        for (let i = 0; i < this.kedu.length; i++) {
          c[1].fillStyle = "#773388";
          c[1].fillRect(leftW - 6, this.kedu[i][0], 4, 1);
          c[1].fillStyle = "#222222";
          c[1].fillRect(leftW - 1, this.kedu[i][0], subBW - leftW, 1);
          c[1].fillStyle = "#AAAAAA";
          c[1].fillText(this.kedu[i][1], leftW - 38, this.kedu[i][0]);
        }
      }
      this.init();
    }

    function _subF() {
      let c = new createCvs("subF", cvsConf.subF);
      this.cvs = c[0];
      this.ctx = c[1];
      this.idx = 0;
      this.big = 10;
      //this.ctx.fillStyle = "#AA0000";
      //this.ctx.fillRect(0, subFH - 1, subFW, 1);
      this.cvs.addEventListener("mousemove", mouseMove, false);
      this.cvs.addEventListener('wheel', xxxx, false);
      this.cvs.addEventListener('dblclick', xx.bind(this), false);

      function xx(e) {
        //alert( " this.idx  " + this.idx ) ;
        if (this.big == 0) {
          frameChange(this.idx, 10);
        } else if (this.big == 10) {
          frameChange(this.idx, 0);
        }
      }
      this.clear = function() {
        this.ctx.clearRect(0, 0, this.cvs.width, this.cvs.height);
      }
    }


    function _stt() {
      let cvs = new createCvs("stt", cvsConf.stt);
      this.cvs = cvs;
      cvs[0].addEventListener("mousemove", mouseMove, false);
      cvs[0].addEventListener('wheel', xxxx, false);
    }

    function _backF() {
      let c = new createCvs("backF", cvsConf.backF);
      this.cvs = c[0];
      this.ctx = c[1];

      /*
		显示cross 以及 K柱相关数据
		*/
      this.dispXdata = function(x, y) {
        if (G.XX[x] == undefined) {
          return;
        }
        let position = F(((G.XX[x] - G.barB) / G.AB[G.curLevel][2]) * c[0].width);
        this.ctx.fillStyle = "#773388";
        this.ctx.fillRect(position + G.halfBarW, 0, 1, this.cvs.height);
        this.ctx.fillRect(0, y, this.cvs.width, 1);
        if (frame[0][0].oo[G.XX[x]] > frame[0][0].cc[G.XX[x]]) {
          this.ctx.fillStyle = "#008888";
        } else {
          this.ctx.fillStyle = "#FF0000";
        }
        let yy = frame[0][0].cvs.height - 15;
        let step = 100;
        let o = 'open:' + frame[0][0].oo[G.XX[x]];
        let h = 'high:' + frame[0][0].hh[G.XX[x]];
        let l = 'low:' + frame[0][0].ll[G.XX[x]];
        let cc = 'close:' + frame[0][0].cc[G.XX[x]];
        this.ctx.fillText(o, 10, yy);
        this.ctx.fillText(h, 10 + step, yy);
        this.ctx.fillText(l, 10 + 2 * step, yy);
        this.ctx.fillText(cc, 10 + 3 * step, yy);
        if (G.XX[x] > 0) {
          let rate = R(((frame[0][0].cc[G.XX[x]] - frame[0][0].cc[G.XX[x] - 1]) / frame[0][0].cc[G.XX[x] - 1]) * 100000) / 1000;
          let r = 'updn:' + rate + '%';
          this.ctx.fillText(r, 10 + 4 * step, yy);
        }
      }

      this.clear = function() {
        this.ctx.clearRect(0, 0, this.cvs.width, this.cvs.height);
      }
    }


    function _backB() {
      let c = new createCvs("backB", cvsConf.backB);
      this.cvs = c[0];
      this.ctx = c[1];
      //cvs[0].style.position = "fixed";
      this.mouseMove = function(x, y) {
        c[1].clearRect(0, 0, backBW, backBH);
        c[1].fillStyle = "#773388";
        c[1].fillRect(x, 0, 1, 600);
        c[1].fillRect(0, y, 1200, 1);
      }
      this.init = function() {}
    }
    /*
	let topNav = new _topNav();
	let rightNav = new _rightNav();
	let botNav = new _botNav();
	let mainF11 = new _mainF();
	let mainB11 = new _mainB();

	mainF11.oo = opt.Y.data[0].y[0].data;
	mainF11.hh = opt.Y.data[0].y[1].data;
	mainF11.ll = opt.Y.data[0].y[2].data;
	mainF11.cc = opt.Y.data[0].y[3].data;
	mainF11.xx = opt.Y.data[0].y[3].data;

	G.datLen = mainF11.oo.length;
	G.curLevel = 0;
	mainF11.line[0] = mainF11.oo; //对于line, bar等属性，请在外部赋值。
	mainF11.line[1] = mainF11.hh;
	*/

    /*
	console.log("length:::::::::::::::::::   " + mainF11.oo.length);
		//alert("this.line.length:  " + mainF11.line.length + " " + mainF11.line[0].length);
		//alert(mainF11.oo[100]);
		//alert(mainF11.line[0][100]);
		//mainF11.hh[100] = 100000;
		//alert(mainF11.line[1][100]);
		//mainF11.drawCandle();
		//console.log(mainF11.PP);
		*/

    /*
	let scrollB = new _scrollB();
	let scrollF = new _scrollF();
	let sub1B11 = new _subB();
	let sub1F11 = new _subF();
	let backF = new _backF();
	let backB = new _backB();

	frame.push([mainF11, mainB11]);
	frame.push([sub1F11, sub1B11]);
	frame.push([backF, backB]);
	frame.push([rightNav, botNav]);
	*/

    function frameInit() {
      let topNav = new _topNav();
      let rightNav = new _rightNav();
      let botNav = new _botNav();
      let mainF11 = new _mainF();
      let mainB11 = new _mainB();

      mainF11.oo = opt.Y.data[0].y[0].data;
      mainF11.hh = opt.Y.data[0].y[1].data;
      mainF11.ll = opt.Y.data[0].y[2].data;
      mainF11.cc = opt.Y.data[0].y[3].data;
      mainF11.xx = opt.Y.data[0].y[3].data;

      G.datLen = mainF11.oo.length;
      G.curLevel = 0;
      mainF11.line[0] = mainF11.oo; //对于line, bar等属性，请在外部赋值。
      mainF11.line[1] = mainF11.hh;

      let scrollB = new _scrollB();
      let scrollF = new _scrollF();
      let sub1B11 = new _subB();
      let sub1F11 = new _subF();
      let backF = new _backF();
      let backB = new _backB();

      frame.push([mainF11, mainB11]);
      frame.push([sub1F11, sub1B11]);
      frame.push([backF, backB]);
      frame.push([rightNav, botNav]);
    }
    frameInit();

    /*
		frameChange(x) 参数：
		n：表示处理的 第几个sub。 n = 1 第一个sub , n=2 第2个sub
		x: 表示删除，还是增加，还是改动。
		   其中，改动分为 放大和缩小。
		   x == 1,2  增加 (在第n个位置增加)
		   x == -1   删除
		   x == 10   放大
		   x == 0    缩小
			n :              0             1           2           3
		frame 的格式：[ [mainF,mainB],[subF,subB],[subF,subB],[subF,subB] ]
		操作上:
		frameChange(2,1) : 在第2个sub前插入一个sub
		frameChange(2,2) : 在第2个sub后插入一个sub
		frameChange(1,-1) : 删除第1个sub
		frameChange(3,10)  : 第3个sub放大
		frameChange(1,0)  : 第1个sub缩小
		*/
    function frameChange(n, x) {
      let i = 0;
      //alert( "frame.length "  + frame.length ) ;
      if (n < 1 || n >= (frame.length - 1)) {
        return -1;
      }
      for (i = 1; i < frame.length - 2; i++) { //清除canvas内容
        frame[i][0].clear();
        frame[i][1].clear();
      }
      if (x == 1 || x == 2) { // 增加一个副图,1在前面增加,2在后面增加
        let subF = new _subF();
        let subB = new _subB();
        if (x == 1) {
          frame.splice(n, 0, [subF, subB]);
        } else if (x == 2) {
          frame.splice(n + 1, 0, [subF, subB]);
        }
      }
      if (x == -1) { // 删除一个副图指标
        document.body.removeChild(frame[n][0].cvs);
        document.body.removeChild(frame[n][1].cvs);
        frame[n][0] = null;
        frame[n][1] = null;
        frame.splice(n, 1);
      }
      if (x == 10) { // 副图放大
        frame[n][0].big = 10;
        frame[n][1].big = 10;
        frame[n][0].cvs.height = H2;
        frame[n][1].cvs.height = H2;
      }
      if (x == 0) { //副图缩小
        frame[n][0].big = 0;
        frame[n][1].big = 0;
        frame[n][0].cvs.height = H3;
        frame[n][1].cvs.height = H3;
      }
      let t = topH + H1 + scrollFH;
      for (i = 1; i < frame.length - 2; i++) { //重绘所有副图指标
        frame[i][0].idx = i;
        frame[i][1].idx = i;
        frame[i][0].cvs.style.top = t;
        frame[i][1].cvs.style.top = t;
        console.log(" i: " + i + " top: " + t);
        frame[i][1].init();
        t = t + frame[i][0].cvs.height;
      }
      //  backF,backB 的高度调整
      frame[i][0].cvs.height = t;
      frame[i][1].cvs.height = t;
      //  rightNav 高度调整
      frame[i + 1][0].cvs.height = t;
      //  botNav 最下面菜单位置调整
      frame[i + 1][1].cvs.style.top = t;
    }

    frameChange(1, 1);
    frameChange(3, 10);
    frameChange(1, 0);
    frameChange(1, 2);
    frameChange(1, 2);
    frameChange(1, 2);
    frameChange(1, 2);
    frameChange(2, -1);
    frameChange(1, 2);
    frameChange(3, 0);

    this.upd = function(opt) {
      if (opt.type == 2) {
        for (let i = 0; i < opt.Y.data.length; i++) {
          if (opt.Y.data[i].position == 'main') {
            frame[0][0].oo.push(opt.Y.data[i].y[0].data[0]);
            frame[0][0].hh.push(opt.Y.data[i].y[1].data[0]);
            frame[0][0].ll.push(opt.Y.data[i].y[2].data[0]);
            frame[0][0].cc.push(opt.Y.data[i].y[3].data[0]);
            //frame[0][0].xx.push(opt.Y.data[i].y[3].data[0]);
            console.log(opt.Y.data[i].y[0].data[0])
            console.log(opt.Y.data[i].y[1].data[0])
            console.log(opt.Y.data[i].y[2].data[0])
            console.log(opt.Y.data[i].y[3].data[0])
            G.datLen = frame[0][0].oo.length;
          }
        }
      }
      //if (opt.Y.data[i].position == 'sub') {}
      G.curLevel = G.curLevel;
      drawAll();
    }


    this.update = function(opt) {
      //function update1(opt) {
      if (opt.type == 1) { //更新，但最后一个只是修改
        for (let i = 0; i < opt.Y.data.length; i++) {
          if (opt.Y.data[i].position == 'main') {
            for (let j = 0; j < opt.Y.data[i].y.length; j++) {
              cD[i][0].y[j].data.pop();
              for (let k = 0; k < opt.Y.data[i].y[j].data.length; k++) {
                cD[i][0].y[j].data.push(opt.Y.data[i].y[j].data[k]);
              }
            }
            let n = 0;
            for (let j = 0; j < cD[i][0].y.length; j++) {
              if (cD[i][0].y[j].type == 'o') {
                cD[i][0].oo = cD[i][0].y[j].data;
                G.datLen = cD[i][0].oo.length;
              } else if (cD[i][0].y[j].type == 'h') {
                cD[i][0].hh = cD[i][0].y[j].data;
              } else if (cD[i][0].y[j].type == 'l') {
                cD[i][0].ll = cD[i][0].y[j].data;
              } else if (cD[i][0].y[j].type == 'c') {
                cD[i][0].cc = cD[i][0].y[j].data;
              } else {
                cD[i][0].other[n] = cD[i][0].y[j];
                n++;
              }
            }
          }
          if (opt.Y.data[i].position == 'sub') {}
        }

        G.curLevel = G.curLevel;
        drawAll();
      } else if (opt.type == 2) { // 更新，最后一个不再修改
        for (let i = 0; i < opt.Y.data.length; i++) {
          if (opt.Y.data[i].position == 'main') {
            for (let j = 0; j < opt.Y.data[i].y.length; j++) {
              for (let k = 0; k < opt.Y.data[i].y[j].data.length; k++) {
                console.log(opt.Y.data[i].y[j].data[k])
                console.log(cD[i][0].y[j].data)
                cD[i][0].y[j].data.push(opt.Y.data[i].y[j].data[k]);
              }
            }
            let n = 0;
            for (let j = 0; j < cD[i][0].y.length; j++) {
              if (cD[i][0].y[j].type == 'o') {
                cD[i][0].oo = cD[i][0].y[j].data;
                G.datLen = cD[i][0].oo.length;
              } else if (cD[i][0].y[j].type == 'h') {
                cD[i][0].hh = cD[i][0].y[j].data;
              } else if (cD[i][0].y[j].type == 'l') {
                cD[i][0].ll = cD[i][0].y[j].data;
              } else if (cD[i][0].y[j].type == 'c') {
                cD[i][0].cc = cD[i][0].y[j].data;
              } else {
                cD[i][0].other[n] = cD[i][0].y[j];
                n++;
              }
            }
          }
          if (opt.Y.data[i].position == 'sub') {}
        }
        //alert("G.datLen " + G.datLen);
        //alert( "O.oo.length: " + O.oo.length );
        G.curLevel = G.curLevel;
        drawAll();
      } else if (opt.type == 0) { //完全更新，不要以前的数据了
        for (let i = 0; i < opt.Y.data.length; i++) {
          if (opt.Y.data[i].position == 'main') {
            let mF = new _mainF();
            let mB = new _mainB();
            mF.y = opt.Y.data[i].y;
            let n = 0;
            mF.other = [];
            for (let j = 0; j < mF.y.length; j++) {
              if (mF.y[j].type == 'o') {
                mF.oo = mF.y[j].data;
                //O.oo = mF.oo;
                G.datLen = mF.oo.length;
              } else if (mF.y[j].type == 'h') {
                mF.hh = mF.y[j].data;
                //O.hh = mF.hh;
              } else if (mF.y[j].type == 'l') {
                mF.ll = mF.y[j].data;
                //O.ll = mF.ll;
              } else if (mF.y[j].type == 'c') {
                mF.cc = mF.y[j].data;
                //O.cc = mF.cc;
              } else {
                mF.other[n] = opt.Y.data[i].y[j];
                n++;
              }
            }
            cD.push([mF, mB]);
          }
          if (opt.Y.data[i].position == 'sub') {
            /*
			let subF = new _subF();
			let subB = new _subB();
			subF.Y = opt.Y.data[i].y;
			cD.push([subF, subB]);
			*/
          }
        }
        G.curLevel = 0;
      }
      let subbB = Object.create(cvsConf.subB);
      let subbF = Object.create(cvsConf.subB);
      cvsConf.subS.push([subbB, subbF]);

      //drawAll();

    } /* end of this.update() */
    //let updateOO = this.update() ;

    /*
	  下面this.update(opt);的参数opt是从 function stockcharts(opt, theme) 中来的。
	  stockcharts(); 是在index.html的<script>中，调用 new barCharts()产生的。
	  在index.html中的opt.type = 0,
	  然后 this.update(opt) 根据 opt.tpye=0 对 cD[] 进行初始化。cD[]，即canvas Display array !!
	  */
    //update1(opt);
    //let mF = cD[0][0]; //mainF  main front
    //let mB = cD[0][1]; //mainB  main back
    //let s1F = cD[1][0]; // s1F sub1F  sub1 front
    //let s1B = cD[1][1]; // s1B sub1B  sub1 back

    //console.log(cvsConf.subS);
    //console.log(cD);
    //console.log(cD[1][0].Y);
    //console.log(cD[0] instanceof _mainF);
    //console.log(cD[1] instanceof _mainF);
    //console.log(cD[1] instanceof _mainB);



    //console.log(Object.entries(opt));


    function _initMA(N, cc) {
      let i = 0;
      let sum = 0;
      let aver;
      let rr = [];
      if (cc.length == 0) {
        return {
          MA: rr,
          sum: 0,
          idx: 0
        }
      }
      if (N <= 0) {
        return -1;
      }
      if (N > cc.length) {
        for (i = 0; i < cc.length - 1; i++) {
          sum = cc[i] + sum;
          rr.push(0);
        }
        return {
          MA: rr,
          sum: (sum - cc[cc.length - 1]), // 前N-1的合值
          idx: cc.length - 1 // cc.length-1
        }
      }
      for (i = 0; i < N - 1; i++) {
        sum = cc[i] + sum;
        rr.push(0);
      }
      if (N == cc.length) {
        sum = sum + cc[N - 1];
        rr.push(R(sum / N * 1000) / 1000);
        i = i + 1;
      } else if (N < cc.length) {
        sum = sum + cc[N - 1];
        rr.push(R(sum / N * 1000) / 1000);
        for (i = N; i < cc.length; i++) {
          sum = sum - cc[i - N] + cc[i];
          aver = R(sum / N * 1000) / 1000;
          rr.push(aver);
        }
      }
      return {
        MA: rr,
        sum: (sum - cc[i - 1]), // 前N-1的合值
        idx: i - 1 // cc.length-1
      };
    }

    function _MA(N, cc) {
      let r = new _initMA(N, cc);
      this.MA = r.MA;
      this.sum = r.sum;
      this.idx = r.idx; // length-1
      this.calcMA = function(x) { // x should be length-1
        if (x <= 0 || cc.length == 0) {
          return;
        }
        if (x >= cc.length) {
          x = cc.length - 1;
        }
        if (x < this.idx) {
          return;
        } else if (x >= this.idx) {
          //console.log("N:i  " + N );
          this.sum = this.sum + cc[this.idx]; // 重新计算 this.idx的MA。
          this.MA.pop();
          this.MA.push(R(this.sum / N * 1000) / 1000); //保留三位小数。

          for (let i = this.idx + 1; i <= x; i++) { //再计算 后面的 MA。
            this.sum = this.sum - cc[i - N] + cc[i];
            this.MA.push(R(this.sum / N * 1000) / 1000);
          }
          this.idx = x;
          this.sum = this.sum - cc[x]; // this.sum 的值会少加最后一个，即cc[x]，因为cc[x]会一直变化。
          /*
			 比如，当前是显示的1f的图表，最后一个1钟的K柱没有定型，收到的数据，是要每次更新最后
			 一个1分钟的K柱，直到第60秒结束，这个K柱才定型。新一个K柱会开始。
			 所以在现实的图上，会看到最后一个K柱一直在变化，等时间到，本K柱结束，新一个K柱开始。
			 */
        }
      }
    }

    function calcYkedu(height, b, e, hh, ll) {
      //let top = 0.05;
      //let down = 0.95;
      let top = 30;
      let down = 30;
      let h = [];
      h = hh.slice(b, e);
      let l = [];
      if (ll == undefined) {
        l = h;
      } else {
        l = ll.slice(b, e);
      }
      let hmax = Math.max.apply(null, h);
      let lmin = Math.min.apply(null, l);
      //let m = (height * (top - down)) / (hmax - lmin);
      let m = (top + down - height) / (hmax - lmin);
      //let n = height * down - m * lmin;
      let n = height - down - m * lmin;
      let kedu = [];
      kedu.push([R(top), hmax]);
      kedu.push([R(height - down), lmin]);
      return [m, n];
    }

    function calcYkedu0(height, hh, ll) {
      if (hh.length == 0) {
        return [0, 0];
      }
      let top = 30;
      let down = 30;
      if (ll == undefined) {
        ll = hh;
      }
      let hmax = Math.max.apply(null, hh);
      let lmin = Math.min.apply(null, ll);
      let m = (top + down - height) / (hmax - lmin);
      let n = height - down - m * lmin;
      return [m, n, hmax, lmin, top, down];
    }

    /*---------------------------------------------------------------------------------------------- */

    /*
	let MA5 = new _MA(5, mF.cc);
	let MA10 = new _MA(10, mF.cc);
	let MA20 = new _MA(20, mF.cc);
	let MA30 = new _MA(30, mF.cc);
	let MA40 = new _MA(40, mF.cc);
	let MA60 = new _MA(60, mF.cc);
	let MA89 = new _MA(89, mF.cc);
	let MA144 = new _MA(144, mF.cc);
	let MA233 = new _MA(233, mF.cc);
	let MA377 = new _MA(377, mF.cc);
	let MA610 = new _MA(610, mF.cc);
	*/

    /* ----------------------------------------------------------------------------------------------------*/





    function drawLine(obj, dd, color) {
      if (dd.length == 0 || G.barB == G.barE) {
        return;
      }
      let ctx = obj.ctx;
      let j = 0;
      let x1, y1;
      let x2, y2;
      let start;
      ctx.beginPath();
      for (let i = G.barB; i < G.barE; i++) {
        if (dd[i] != 0) {
          start = i;
          break;
        }
      }
      let v = R(obj.mn[0] * dd[start] + obj.mn[1]);
      x1 = P[start - G.barB] + 0.5;
      y1 = v + 0.5;
      ctx.moveTo(x1, y1);
      for (let i = start + 1; i < G.barE; i++) {
        /*
		   while (P[i - G.barB] == P[start - G.barB]) {
			 i++;
		   }
		   */

        v = R(obj.mn[0] * dd[i] + obj.mn[1]);
        x2 = P[i - G.barB] + 0.5;
        y2 = v + 0.5;

        ctx.lineTo(x2, y2);
        x1 = x2;
        y1 = y2;

      }
      ctx.strokeStyle = color;
      ctx.stroke();
      ctx.closePath();
    }


    function drawCandle(obj) {
      if (G.barB == G.barE) {
        return;
      }
      let oo = [],
        hh = [],
        ll = [],
        cc = [];
      let idxB;
      let idxE;
      let posB;
      let position,
        halfBarW = (G.barW - 1) / 2;
      let cvs = obj.cvs;
      let ctx = obj.ctx;
      ctx.clearRect(0, 0, cvs.width, cvs.height);

      let o, h, l, c, dif1, dif2;
      let PP = [];

      P.length = 0;

      posB = -1;
      idxB = G.barB;

      for (let i = G.barB; i < G.barE; i++) {
        position = F(((i - G.barB) / G.AB[G.curLevel][2]) * cvs.width);
        P[i - G.barB] = position + halfBarW;
        if (position == posB && i != (G.barE - 1)) {
          continue;
        } else {
          let h1 = mF.hh.slice(idxB, i + 1)
          let l1 = mF.ll.slice(idxB, i + 1)
          let hhh = Math.max.apply(null, h1);
          let lll = Math.min.apply(null, l1);

          hh.push(hhh);
          ll.push(lll);
          oo.push(mF.oo[idxB]);
          cc.push(mF.cc[i]);

          PP.push(position + halfBarW);

          posB = position;
          idxB = i + 1;
        }
      }
      obj.mn = calcYkedu0(cvs.height, hh, ll);
      for (let i = 0; i < hh.length; i++) {
        if (halfBarW != 0) {
          o = R(obj.mn[0] * oo[i] + obj.mn[1]);
          c = R(obj.mn[0] * cc[i] + obj.mn[1]);
          dif1 = c - o;
          if (dif1 == 0) {
            dif1 = 1;
          }
        }
        h = R(obj.mn[0] * hh[i] + obj.mn[1]);
        l = R(obj.mn[0] * ll[i] + obj.mn[1]);
        dif2 = l - h;
        if (dif2 == 0) {
          dif2 = 1;
        }

        if (cc[i] < oo[i]) {
          ctx.fillStyle = "#008888";
          if (halfBarW != 0) {
            ctx.fillRect(PP[i] - halfBarW, o, G.barW, dif1);
            ctx.fillRect(PP[i], h, 1, dif2);
          } else {
            ctx.fillRect(PP[i], h, 1, dif2);
          }
        } else {
          ctx.fillStyle = "#880000";
          if (halfBarW != 0) {
            ctx.fillRect(PP[i] - halfBarW, c, G.barW, 0 - dif1);
            ctx.fillRect(PP[i], h, 1, dif2);
          } else {
            ctx.fillRect(PP[i], h, 1, dif2);
          }
        }
      }

      /*
	  drawLine(obj, MA5.MA, "#E7E7E7");
	  drawLine(obj, MA10.MA, "#DCDC0A");
	  drawLine(obj, MA20.MA, "#FF00FF");
	  drawLine(obj, MA30.MA, "#FF0000");
	  drawLine(obj, MA40.MA, "#00F000");
	  drawLine(obj, MA60.MA, "#787878");
	  drawLine(obj, MA89.MA, "#FF0000");
	  drawLine(obj, MA144.MA, "#FF8000");
	  drawLine(obj, MA233.MA, "#808000");
	  drawLine(obj, MA377.MA, "#800080");
	  drawLine(obj, MA610.MA, "#008080");
	  */

    }


    this.drawCandle0 = function() {
      //   position = F(F(x * G.AB[G.curLevel][2] / mainFW) * mainFW / G.AB[G.curLevel][2]);
      /*
		 反求：
		 如果 G.AB[G.curLevel][2] 比 mainFW 小的话：
		 position = F(F(x * G.AB[G.curLevel][2] / mainFW) * mainFW / G.AB[G.curLevel][2]);
		 x:在屏上取得的X坐标
		 */
    }

    function drawAll() {

      /*
	  MA5.calcMA(G.datLen);
	  MA10.calcMA(G.datLen);
	  MA20.calcMA(G.datLen);
	  MA30.calcMA(G.datLen);
	  MA40.calcMA(G.datLen);
	  MA60.calcMA(G.datLen);
	  MA89.calcMA(G.datLen);
	  MA144.calcMA(G.datLen);
	  MA233.calcMA(G.datLen);
	  MA377.calcMA(G.datLen);
	  MA610.calcMA(G.datLen);
				//drawCandle(cD[0][0]);
				*/

      frame[0][0].clear();
      frame[0][0].drawCandle();
      //console.log(frame[0][0].PP);
      //console.log(G.PP);
      //console.log(G.XX);

      /*
	  let uu = MA5.MA.slice(G.barB, G.barE);
	  s1F.mn = calcYkedu0(s1F.cvs.height, uu, uu);
	  s1F.ctx.clearRect(0, 0, subFW, subFH);
				//drawLine(sub1F, MA60.MA, "#787878");
				//drawLine(sub1F, MA30.MA, "#787878");
				//drawLine(cD[1][0], MA10.MA, "#787878");
	  drawLine(s1F, MA10.MA, "#787878");
	  */

    }

    /* ------------ websocket ------------------------------------------------------------------------- */

    const sock = new WebSocket('ws://192.168.3.211:80/api/push/ws');

    // Connection opened
    sock.addEventListener('open', function(e) {
      //sock.send('{"action":"unsubs","stamp":"x","data":["TA703","TA705"]}');
      //sock.send('{"action":"subs","data":["TA703","TA705:5F"]}');
      sock.send('{"action":"subs","data":["TA703","TA705:5s"]}');
      //sock.send('{"action":"subs","data":["TA703","TA705:1s","TA705:2s"]}');
    });

    // Listen for messages
    sock.addEventListener('message', function(e) {
      console.log('Message from server', e.data);
      /*
	  let obj = JSON.parse(e.data)
	  let ary = [];
	  ary = obj['data'].split(' ');
	  */
      opt_oo.length = 0;
      opt_hh.length = 0;
      opt_ll.length = 0;
      opt_cc.length = 0;
      /*
	  opt_oo.push(parseFloat(ary[7]));
	  opt_hh.push(parseFloat(ary[11]));
	  opt_ll.push(parseFloat(ary[15]));
	  opt_cc.push(parseFloat(ary[19]));
	  */
      opt_oo.push(100);
      opt_hh.push(110);
      opt_ll.push(90);
      opt_cc.push(105);
      option.type = 2;

      //update1(option);
      //console.log( ary ) ;

    });

    sock.addEventListener('error', function(e) {
      console.log('Message from server errrrrrrrrrrrrrrrrrrrrrrrr', e.data);
    });

    // ping/pong heartbeat
    //setInterval('sock.send(\'{"action":"ping"}\')', 5000);

    /* ----------------------------------------------------------------------------------------------- */

    function keyPress(e) {
      let keyID = e.keyCode ? e.keyCode : e.which;
      if (keyID === 38 || keyID === 87) { // up arrow and W
        console.log("up");
        e.preventDefault();
        if (G.curLevel > 0) {
          G.curLevel = G.curLevel - 1;
          drawAll();
        }
        e.preventDefault();
      }
      if (keyID === 40 || keyID === 83) { // down arrow and S
        console.log("down");
        e.preventDefault();
        if (G.curLevel < G.maxLevel) {
          G.curLevel = G.curLevel + 1;
          drawAll();
        }
        e.preventDefault();
      }
      if (keyID === 37 || keyID === 65) { // left arrow and A
        //setBE(-G.curLevel);
        G.barB = G.barB - G.curLevel;
        drawAll();
        e.preventDefault();
      }
      if (keyID === 39 || keyID === 68) { // right arrow and D
        /*
			if (G.barE < O.hh.length) {
		   G.barE++;
		   G.barB++;
		  };
		  */
        //setBE(G.curLevel);
        G.barB = G.barB + G.curLevel;
        drawAll();
        e.preventDefault();
      }
    }

    function xxxx(e) {
      e.preventDefault();
      console.log(e.deltaX);
      console.log(e.deltaY);
      if (e.deltaY > 0) { // 向下
        G.barB = G.barB - 50;
      } else {
        G.barB = G.barB + 50;
      }
      drawAll();
      e.preventDefault();
    }

    function mouseMove(e) {
      let backF = frame[frame.length - 2][0];
      let mousePos = getMousePos(backF.cvs, e);
      //backF.mouseMove(mousePos.x, mousePos.y);
      backF.clear();
      backF.dispXdata(mousePos.x, mousePos.y);
      /*
		frame[0][1] 指mainB.
		*/
      frame[0][1].clear();
      frame[0][1].dispScale();
      frame[0][1].dispYdata(mousePos.y);
    }

    function getMousePos(cvs, e) {
      let r = cvs.getBoundingClientRect();
      return {
        x: e.clientX - r.left * (cvs.width / r.width),
        y: e.clientY - r.top * (cvs.height / r.height)
      }
    }


    cvs.addEventListener('keydown', keyPress, true);
    cvs.focus();
    // key eee - use window as object
    window.addEventListener('keydown', keyPress, true);

    //cvs.addEventListener("mousedown", doMouseDown, false);
    //cvs.addEventListener('mousemove', doMouseMove, false);
    //cvs.addEventListener('mouseup', doMouseUp, false);
    cvs.addEventListener('wheel', xxxx, false);
    //cvs.addEventListener("mousemove", yyyy, false);
    //cvs.addEventListener("click", kkkkk, false);
    drawAll();


  }
  return stockcharts;
})));
