;try {
/* module-key = 'com.adaptavist.selectacular:web-item-resource', location = 'js/web-item.js' */
if(typeof window.Adaptavist==="undefined"){window.Adaptavist={}}Adaptavist.Selectacular=(function(b,a){a.SELECTACULAR_PARAM="selectacular";a.fragmentsUrl=function(d){var c=location.search.replace(new RegExp(a.SELECTACULAR_PARAM+"(=[^&]*)?"),"");if(d){c+=(c?"&":"?")+a.SELECTACULAR_PARAM+"="+d}if(c==="?"){c=""}return location.pathname+c+location.hash};a.fragmentsEnable=function(c){location.replace(a.fragmentsUrl(!!c))};a.fragmentsStart=function(){if(a.fragmentsActive&&a.restart){var c=a.getTool("close-selectacular");c.desc="Exit Design Mode";c.action=a.fragmentsExit;a.restart()}else{a.fragmentsEnable(true)}};a.fragmentsExit=function(){a.fragmentsEnable(false)};b(document).on("click",".selectacular-web-item",function(c){c.preventDefault();a.fragmentsStart()});b(document).ready(function(){AJS.I18n.get("com.adaptavist.selectacular");if(location.search.indexOf(a.SELECTACULAR_PARAM+"=true")>=0){window.setTimeout(function(){a.fragmentsStart()},100)}});return a})(jQuery,Adaptavist.Selectacular||{});
} catch (err) {
    if (console && console.log && console.error) {
        console.log("Error running batched script.");
        console.error(err);
    }
}

;
