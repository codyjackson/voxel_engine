define(['angular', './module-declaration'], function(angular, colorPickerModule){
    colorPickerModule
    .factory('colorUtils', [function(){
        function isBetween(x, lower, upper) {
            return (lower <= x ) && (x < upper);
        }

        function Hsv(h, s, v) {
            this.h = h;
            this.s = s;
            this.v = v;
        }
        Hsv.prototype.update = function(h, s, v) {
            this.h = h;
            this.s = s;
            this.v = v;
        };

        Hsv.prototype.toRgb = function() {
            //Implemented using http://en.wikipedia.org/wiki/HSL_and_HSV#From_HSV
            var C = this.v * this.s;
            var dH = this.h / 60;
            var X = C*(1 - Math.abs((dH % 2) - 1));
            var r, g, b;
            if(isBetween(dH, 0, 1)) {
                r = C;
                g = X;
                b = 0;
            } else if(isBetween(dH, 1, 2)) {
                r = X;
                g = C;
                b = 0;
            } else if(isBetween(dH, 2, 3)) {
                r = 0;
                g = C;
                b = X;
            } else if(isBetween(dH, 3, 4)) {
                r = 0;
                g = X;
                b = C;
            } else if(isBetween(dH, 4, 5)) {
                r = X;
                g = 0;
                b = C;
            } else if(isBetween(dH, 5, 6)) {
                r = C;
                g = 0;
                b = X;
            } 
            var m = this.v - C;
            return new Rgb(255*(r + m), 255*(g + m), 255*(b + m));
        };

        function Rgb(r, g, b) {
            this.r = r;
            this.g = g;
            this.b = b;
        }

        Rgb.prototype.toRgbString = function() {
            return 'rgb(' + Math.floor(this.r) + ',' + Math.floor(this.g) + ',' + Math.floor(this.b) + ')';
        };

        Rgb.prototype.toHexString = function() {
            var hex = ((this.r << 16) | (this.g << 8) | this.b).toString(16).substr(-6);
            var paddedHex = ('000000' + hex).substr(-6);
            return '#' + paddedHex;
        };

        Rgb.prototype.toHsv = function(){
            //Using algorithm described at: http://www.javascripter.net/faq/rgb2hsv.htm
            var r = this.r/255;
            var g = this.g/255;
            var b = this.b/255;

            var minRGB = Math.min(r, g, b);
            var maxRGB = Math.max(r, g, b);
            var dMaxMin = maxRGB - minRGB;

            if(minRGB === maxRGB) {
                return new Hsv(0, 0, minRGB);
            }

            var d = (r==minRGB) ? g-b : ((b==minRGB) ? r-g : b-r);
            var h = (r==minRGB) ? 3 : ((b==minRGB) ? 1 : 5);
            var computedH = 60*(h - d/(maxRGB - minRGB));
            var computedS = (maxRGB - minRGB)/maxRGB;
            var computedV = maxRGB;

            return new Hsv(computedH, computedS, computedV);
        };

        function hexToRgb(hexInteger) {
            var r = (hexInteger & 0xFF0000) >> 16;
            var g = (hexInteger & 0x00FF00) >> 8;
            var b = hexInteger & 0x0000FF;
            return new Rgb(r, g, b);
        }

        return {
           Hsv: Hsv,
           Rgb: Rgb,
           hexToRgb: hexToRgb
        };
    }]);
});