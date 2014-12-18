define(['./module-declaration', './color-utils'], function(colorPickerModule, colorUtils){

    colorPickerModule.directive('colorSvPicker', ['colorUtils', '$parse', function(colorUtils, $parse){
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
            scope: {
                hue: '=',
                saturation: '=',
                value: '='
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

                scope.$watch('hue', function(){
                    for(var y = 0; y < rect.height; ++y) {
                        for(var x = 0; x < rect.width; ++x) {
                            var i = ((rect.width * y) + x) * numberOfColorChannels;
                            var h = scope.hue;
                            var s = calculateSaturation(x);
                            var v = calculateValue(y);
                            var hsv = new colorUtils.Hsv(h, s, v);
                            var rgb = hsv.toRgb();
                            imageData[i+0] = rgb.r;
                            imageData[i+1] = rgb.g;
                            imageData[i+2] = rgb.b;
                            imageData[i+3] = 255;
                        }
                    }
                    context.putImageData(image, 0, 0);
                });

                var indicatorElement = $(element).find('.indicator');
                var indicatorPos = {x: 0, y: 0};

                function calculateSaturation(x) {
                    return x / (rect.width - 1);
                }

                function calculateX(saturation) {
                    return saturation * (rect.width - 1);
                }

                function calculateValue(y) {
                    return 1 - (y / (rect.height - 1));
                }

                function calculateY(value) {
                    return (value - 1) * -(rect.height - 1);
                }

                function updateModelsFromMouseEvent(event) {
                    scope.$apply(function(){
                        indicatorPos.x = constrain(event.pageX - $(element).offset().left, 0, rect.width);
                        indicatorPos.y = constrain(event.pageY - $(element).offset().top, 0, rect.height);
                        scope.saturation = constrain(calculateSaturation(indicatorPos.x), 0, 1);
                        scope.value = constrain(calculateValue(indicatorPos.y), 0, 1);
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

                function render() {
                    indicatorElement.css({left: indicatorPos.x, top: indicatorPos.y});
                }

                scope.$watch('saturation', function(saturation){
                    indicatorPos.x = calculateX(saturation);
                    render();
                });

                scope.$watch('value', function(value){
                    indicatorPos.y = calculateY(value);
                    render();
                });
            },
            controller: ['$scope', function($scope){
                $scope.chooseColor = function($event) {
                    console.log('hello');
                };
            }]
        };
    }]);
});