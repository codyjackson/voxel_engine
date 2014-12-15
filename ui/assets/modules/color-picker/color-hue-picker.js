define(['angular', './color-picker', './color-utils'], function(angular, colorPicker, colorUtils){
    colorPicker.directive('colorHuePicker', ['colorUtils', '$timeout', '$parse', function(colorUtils, $timeout, $parse){
        function constrain(x, low, high) {
            if(x < low) {
                return low;
            }

            if(x > high) {
                return high;
            }

            return x;
        }
        return {
            restrict: 'A',
            template: '<canvas></canvas><div class="indicator"></div>',
            transclude: true,
            replace: false,
            priority: 1000000,
            scope: {
                hue: '='
            },
            link: function(scope, element, attrs) {
                var canvasElement = $(element).find('canvas')[0];
                var context = canvasElement.getContext('2d');
                var rect = {
                    width: $(element).innerWidth(),
                    height: $(element).innerHeight()
                };
                context.canvas.width = rect.width;
                context.canvas.height = rect.height;

                var image = context.createImageData(rect.width, rect.height);
                var imageData = image.data;
                var numberOfColorChannels = 4;

                for(var i = 0; i < imageData.length; ++i) {
                    imageData[i] = 255;
                }

                var dy = 360.0 / rect.height;
                var s = 1;
                var v = 1;
                var hsv = new colorUtils.Hsv(0, s, v);
                for(var y = 0; y < rect.height; ++y) {
                    var h = (360 - (y * dy)) % 360;
                    hsv.update(h, s, v);
                    for(var x = 0; x < rect.width; ++x) {
                        var i = ((rect.width * y) + x) * numberOfColorChannels;
                        var rgb = hsv.toRgb();
                        imageData[i+0] = rgb.r;
                        imageData[i+1] = rgb.g;
                        imageData[i+2] = rgb.b;
                        imageData[i+3] = 255;
                    }
                }
                context.putImageData(image, 0, 0);

                if(!scope.hue) {
                    scope.hue = 0;
                }
                var indicatorY = scope.hue / dy;
                var indicatorElement = $(element).find('.indicator');
                function invertY(y){
                    return rect.height - y;
                }

                function updateModelsFromMouseEvent(event) {
                    scope.$apply(function(){
                        indicatorY = constrain(event.pageY - $(element).offset().top, 0, rect.height);
                        scope.hue = (invertY(indicatorY) * dy) % 360;
                    });
                }
                $(element).on('mousedown', function(event){
                    $(document).on('mousemove', updateModelsFromMouseEvent);
                    $(document).on('mouseup', function(){
                        $(document).off('mousemove');
                        $(document).off('mouseup');
                    });
                    updateModelsFromMouseEvent(event);
                });
                scope.$watch('hue', function(hue){
                    if(indicatorY !== 0 || hue !== 0) {
                        indicatorY = invertY(constrain(hue / dy, 0, rect.height));
                    }
                    indicatorElement.css({ top: indicatorY + 'px'});
                });
            }
        };
    }]);
});