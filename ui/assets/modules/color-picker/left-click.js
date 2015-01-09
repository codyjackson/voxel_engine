define(['./module-declaration',], function(colorPickerModule){
    colorPickerModule.directive('leftClick', ['$parse', function($parse){
        return {
            restrict: 'A',
            link: function(scope, element, attrs) {
                var fn = null;
                scope.$watch('leftClick', function(expression){ 
                    fn = $parse(attrs['leftClick']);
                });

                element.on('click', function(ev){
                    if(ev.which !== 1) {
                        return;
                    }
                    ev.preventDefault();
                    scope.$apply(function(){
                        fn(scope);
                    });
                });
            }
        };
    }]);
});