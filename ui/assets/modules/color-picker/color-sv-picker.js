define(['./color-picker', './color-utils'], function(colorPicker, colorUtils){

    colorPicker.directive('colorSvPicker', ['colorUtils', '$parse', function(colorUtils, $parse){
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
            require: '?ngModel',
            scope: {
                ngModel: '=',
                hue: '=',
                saturation: '=',
                value: '='
            },
            link: function(scope, element, attrs, ngModelController) {
                if(!ngModelController) {
                    throw "Couldn't find ng-model";
                }
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

                function calculateSaturation(x) {
                    return x / (rect.width - 1);
                }

                function calculateValue(y) {
                    return 1 - (y / (rect.height - 1));
                }

                scope.$watch('hue', function(){
                    for(var i = 0; i < imageData.length; ++i) {
                        imageData[i] = 255;
                    }
                    console.log("test:" + scope.hue);
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
                ngModelController.$render = function(){
                    indicatorElement.css({left: indicatorPos.x, top: indicatorPos.y});
                };

                scope.saturation = 200;
                var indicatorPos = {x: 0, y: 0};
                function updateModelsFromMouseEvent(event) {
                    indicatorPos.x = event.pageX - $(element).offset().left;
                    indicatorPos.y = event.pageY - $(element).offset().top;
                    ngModelController.$render();
                    scope.saturation = calculateSaturation(indicatorPos.x);
                    scope.$apply();
                }
                $(element).on('mousedown', function(event){
                    $(document).on('mousemove', updateModelsFromMouseEvent);
                    $(document).on('mouseup', function(){
                        $(document).off('mousemove');
                        $(document).off('mouseup');
                    });
                    updateModelsFromMouseEvent(event);
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