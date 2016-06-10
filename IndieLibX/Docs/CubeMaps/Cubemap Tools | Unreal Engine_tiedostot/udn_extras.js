var clickCounter = 0;
var params = [];
var start = 1;
var page = "";
var skills = [];
var versions = [];
var tags = [];
var bVideoPlayer = false;
var bPlayerReady = false;
var timer = null;
dataLayer = [];
$(document).ready(function () {
    $.ajax({
        url: headerURL,
        context: document.body
    }).done(function (data) {
        $("#head").html(data);
		customizeEpicHeader();
    }).fail(function( jqXHR, textStatus, errorThrown ) {
		$('head').append('<link rel="stylesheet" href="../CSS/udn_header.css" type="text/css" />');
	});
	PageReady();
});

var customizeEpicHeader = function() {
    if (typeof _epicGamesNavObserver !== 'undefined' && _epicGamesNavObserver.subject) {
        //apply your customizations here
		_epicGamesNavObserver.set('onStickyShow', function() { updateSiderbarHeight() });
		_epicGamesNavObserver.set('onStickyHide', function() { updateSiderbarHeight() });
		var navPanel = document.getElementById('NavPanelList');
		if(navPanel != null)
		{
			updateSiderbarHeight();
		}
    } else {
        setTimeout(function() {
            customizeEpicHeader();
        }, 500);
       //console.log("_epicGamesNavObserver could not be found...");
    }
};

function PageReady()
{	
    var sitemap = document.getElementById('sitemap');
	if(sitemap != null)
	{
		CollapsibleLists.applyTo(document.getElementById('sitemap'), false, function () {
		});
	}
    $('#snippet_image pre').each(function () {
        $(this).removeClass('prettyprint');
    });
    prettyPrint();
	
	if ($.fn.rrssb)
	{
		$('.rrssb-buttons').rrssb({
			// required:
			title: $('title').html(),
			url: location.href,
		});
	}

	//if($('#tracking').length > 0)
	//{
	//	$('#tracking').html('<img src="' + trackingURL + '" width="1" height="1" />');
	//}
	//else
	//{
	//	$('body').append('<div id="tracking" style="display:inline-block;"><img src="' + trackingURL + '" width="1" height="1" /></div>');
	//}
    $('#sidebar').hide();
    params = parseParamsFromUrl();
    if (params['start']) {
        start = parseInt(params['start']);
    }
    if (params['page']) {
        page = params['page'];
    }
    if (params['skills']) {
        skills = params['skills'].split(',');
		for(s=0; s < skills.length; s++)
		{
			addSkillFilter(skills[s]);
		}
    }
    if (params['versions']) {
        versions = params['versions'].split(',');
		for(v=0; v < versions.length; v++)
		{
			addVersionFilter(versions[v].replace('.', '_'));
		}
    }
    if (params['tags']) {
        tags = params['tags'].split(',');
		for(v=0; v < tags.length; v++)
		{
			addTag('tag_' + tags[v].replace(' ', '_').toLowerCase());
		}
    }
	updateFilters();

    setupDocPlaylistNav(params);
    if (bVideoPlayer) {
        getVideoSeries(params);
    }

    $(document).click(function (e) {
        if ($(e.target).closest('.searchicon').length === 0) {
            $('#searchhelp').hide();
        }
        else if ($(e.target).closest('#searchhelp').length === 0) {
            $('#searchhelp').toggle();
        }
        if ($(e.target).closest('#sidebar').length === 0) {
            $('#sidebar').hide();
        }
    });
	
	$(".comparison_slider").each(function(){
		$(this).twentytwenty();
	});

    $.fn.qtip.zindex = 200000;

    $('.tooltip').each(function () {
        var myclass = $(this).attr('class');
        var classes = $(this).attr('class').split(' ');
        if (classes[1] != "") {
            var id = classes[1];
            var html = $('#' + id + '_tipcontent').html();
            $(this).qtip({
                content: html,
                position: {
                    my: 'top left',  // Position my top left...
                    at: 'bottom left', // at the bottom right of...
                    target: $('.' + id + '> .role'), // my target
                },
                show: {
                    effect: function (offset) {
                        $(this).effect("fade", 100); // "this" refers to the tooltip
                    }
                },
                hide: {
                    effect: function (offset) {
                        $(this).effect("fade", 100); // "this" refers to the tooltip
                    }
                }
            });
        }
    });

    //$('.topiclinktip').each(function () {
    //    var myclass = $(this).attr('class');
    //    var classes = $(this).attr('class').split(' ');
    //    if (classes[1] != "") {
    //        var id = classes[1];
    //        var html = $('#' + id + '_tipcontent').html();
    //        $(this).qtip({
    //            content: html,
    //            position: {
    //                my: 'left top',  // Position my top left...
    //                at: 'right middle', // at the bottom right of...
    //            },
    //            show: {
    //                effect: function (offset) {
    //                    $(this).effect("fade", 100); // "this" refers to the tooltip
    //                }
    //            },
    //            hide: {
    //                effect: function (offset) {
    //                    $(this).effect("fade", 100); // "this" refers to the tooltip
    //                }
    //            }
    //        });
    //    }
    //});

    //$('A').each(function () {
    //    var menuElement = $(this).closest('.menu');
    //    var nodeElement = $(this).closest('.node');
    //    if (menuElement.length > 0)
    //    {
    //        if ($(this).attr('title') != "" && !$(this).hasClass('topiclinktip')) {
    //            $(this).qtip({
    //                content: {
    //                    attr: 'title'
    //                },
    //                position: {
    //                    my: 'left top',  // Position my top left...
    //                    at: 'right top', // at the bottom right of...
    //                    adjust: {
    //                        x: 8,
    //                        y: 8
    //                    }
    //                },
    //                show: {
    //                    effect: function (offset) {
    //                        $(this).effect("fade", 100); // "this" refers to the tooltip
    //                    }
    //                },
    //                hide: {
    //                    effect: function (offset) {
    //                        $(this).effect("fade", 100); // "this" refers to the tooltip
    //                    }
    //                }
    //            });
    //        }
    //    }
    //    else if (nodeElement.length > 0) {
    //        //if ($('#' + $(this).parent().attr('id').substring(4, $(this).parent().attr('id').length)).length > 0)
    //        //{
    //        //    $(nodeElement).qtip({
    //        //        content: {
    //        //            text: $('.node_description').html(),
    //        //            title: $(this).find('.node > .titlebar_text').html()
    //        //        },
    //        //        position: {
    //        //            //target: nodeElement,
    //        //            my: 'top left',  // Position my top left...
    //        //            at: 'bottom left', // at the bottom right of...
    //        //            adjust: {
    //        //                x: 10,
    //        //                y: 8
    //        //            }
    //        //        },
    //        //        style: {
    //        //            classes: "qtip-dark qtip-compact"
    //        //        },
    //        //        show: {
    //        //            solo: true
    //        //        }
    //        //    });
    //        //}
    //    }
    //    else
    //    {
    //        //if ($(this).attr('title') != "" && !$(this).hasClass('topiclinktip')) {
    //        //    $(this).qtip({
    //        //        content: {
    //        //            attr: 'title'
    //        //        },
    //        //        position: {
    //        //            my: 'top left',  // Position my top left...
    //        //            at: 'bottom left', // at the bottom right of...
    //        //            adjust: {
    //        //                x: 4,
    //        //                y: 8
    //        //            }
    //        //        },
    //        //        show: {
    //        //            effect: function (offset) {
    //        //                $(this).effect("fade", 100); // "this" refers to the tooltip
    //        //            }
    //        //        },
    //        //        hide: {
    //        //            effect: function (offset) {
    //        //                $(this).effect("fade", 100); // "this" refers to the tooltip
    //        //            }
    //        //        }
    //        //    });
    //        //}
    //    }
    //});

    $('.pin_container').each(function () {
        var id = $(this).attr('id').substring(4, $(this).attr('id').length);
        if ($('#' + id).find('.desc-cell').length > 0 &&
            $('#' + id).find('.desc-cell').text().trim() != "") {
            $(this).qtip({
                content: {
                    text: $('#' + id).find('.desc-cell').html(),
                    title: $(this).find('.titlebar_pin').text() + "<div style=\"color:#e0e0e0;font-size:0.75em;font-weight:bold;display:inline-block;\">(" + $('#' + id).find('.name-cell-arguments').text().trim() + ")</div>"
                },
                position: {
                    //target: nodeElement,
                    my: 'top left',  // Position my top left...
                    at: 'bottom left', // at the bottom right of...
                    adjust: {
                        x: 24,
                        y: 8
                    }
                },
                style: {
                    classes: "qtip-dark qtip-compact"
                },
                show: {
                    effect: function (offset) {
                        $(this).effect("fade", 100); // "this" refers to the tooltip
                    }
                },
                hide: {
                    effect: function (offset) {
                        $(this).effect("fade", 100); // "this" refers to the tooltip
                    }
                }
                //show: {
                //    solo: true,
                //    delay: 250
                //},
                //hide: {
                //    delay: 100
                //},
                //events: {
                //    hide: function(event, api) {
                //        $(nodeElement).mouseenter();
                //    }
                //}
            });
        }
    });
}


function parseParamsFromUrl() {
    var params = {};
    var parts = window.location.search.substr(1).split('&');
    for (var i = 0; i < parts.length; i++) {
        var keyValuePair = parts[i].split('=');
        var key = decodeURIComponent(keyValuePair[0]);
        params[key] = keyValuePair[1] ?
            decodeURIComponent(keyValuePair[1].replace(/\+/g, ' ')) :
            keyValuePair[1];
    }
    return params;
}

function addSearchTerm(term) {
    var spacer = "";
    if ($('input[name=\'q\']').val() != "") {
        spacer = " ";
    }
    $('input[name=\'q\']').val($('input[name=\'q\']').val() + spacer + term);
}

function executeSearch(term) {
    $('input[name=\'q\']').val(term);
    $('#searchform').submit();
}

function getVideoSeries(params) {
    if (params["series"]) {
        var playlistid = params["series"];
        var videoid = null;
        if (params["video"]) {
            videoid = params["video"];
        }
        $('#player').ytv({
            async: false,
            videoid: videoid,
            element: null,
            user: 'UnrealDevelopmentKit',
            playlist: playlistid,
            fullscreen: false,
            accent: '#007EBF',
            controls: true,
            annotations: true,
            autoplay: false,
            chainVideos: false,
            browsePlaylists: false,
            wmode: 'opaque',
            events: {
                videoReady: function () {
                },
                stateChange: function () { }
            }
        });
    }
    else if (params["video"]) {
        if (bPlayerReady) {
            getVideoPlayer('#player', params["video"]);
            var stillgoing = true;
        }
        else {
            timer = setTimeout('getVideoPlayer("#player", params["video"])', 100);
        }
    }
}

function toggleChaining() {
    createCookie('video_chain', !$('.ytv-button.chain').hasClass('enabled'), 365);
    if ($('.ytv-button.chain').hasClass('enabled')) {
        $('.ytv-button.chain').removeClass('enabled');
    }
    else {
        $('.ytv-button.chain').addClass('enabled');
    }
}

function getVideoInfo(videoid) {
    $.ajax({
        url: "https://www.googleapis.com/youtube/v3/videos?id=" + videoid + "&part=snippet&channelId=UCBobmJyzsJ6Ll7UbfhI4iwQ&key=AIzaSyBN3Ni98mPCgkd8U9T9ZfZXY-nQD-yo2gU",
        dataType: "jsonp",
        success: function (data) { parseVideoResults(data, videoid); }
    });
}

function getVideoPlayer(element, videoid) {
    if (timer) {
        clearTimeout(timer);
    }
    $.ajax({
        url: "https://www.googleapis.com/youtube/v3/videos?id=" + videoid + "&part=snippet&channelId=UCBobmJyzsJ6Ll7UbfhI4iwQ&key=AIzaSyBN3Ni98mPCgkd8U9T9ZfZXY-nQD-yo2gU",
        dataType: "jsonp",
        success: function (data) { parseVideoPlayerResults(data, element, videoid); }
    });
}

function getPlaylistInfo(videoid, playlistid) {
    $.ajax({
        url: "https://www.googleapis.com/youtube/v3/playlists?playlistId=" + playlistid + "&part=snippet&key=AIzaSyBN3Ni98mPCgkd8U9T9ZfZXY-nQD-yo2gU",
        dataType: "jsonp",
        success: function (data) { parsePlaylistResults(data, videoid); }
    });
}

function getChannelGallery() {
    bParsed = false;
    var pageToken = "";
    if (page != "") {
        pageToken = "&pageToken=" + page;
    }
    $.ajax({
        url: 'https://www.googleapis.com/youtube/v3/search?q=%22video%20tutorial%20series%22&type=playlist&part=snippet&maxResults=50' + pageToken + '&channelId=UCBobmJyzsJ6Ll7UbfhI4iwQ&key=AIzaSyBN3Ni98mPCgkd8U9T9ZfZXY-nQD-yo2gU',
        dataType: "jsonp",
        success: function (data) { parseChannelGallery(data); }
    });
}

function getFeaturedSeries() {
    bFeaturedParsed = false;
    $.ajax({
        url: 'https://www.googleapis.com/youtube/v3/search?q=%22video%20tutorial%20series%22&type=playlist&order=rating&part=snippet&maxResults=12&channelId=UCBobmJyzsJ6Ll7UbfhI4iwQ&key=AIzaSyBN3Ni98mPCgkd8U9T9ZfZXY-nQD-yo2gU',
        dataType: "jsonp",
        success: function (data) { parseFeaturedSeries(data); }
    });
}

function parseVideoResults(data, videoid) {
    var title = data.entry.title.$t;
    var description = data.entry.media$group.media$description.$t;
    var viewcount = data.entry.yt$statistics.viewCount;
    var author = data.entry.author[0].name.$t;
    $('DIV[name="' + videoid + '_title"]').each(function () {
        $(this).html(title);
    });
    //var existingDesc = $('#' + videoid + '_description').html();
    //if (existingDesc != "")
    //{
    //    $('#' + videoid + '_description_text').html(description);
    //}
    //$('#' + videoid).mouseenter(function () {
    //    $('#' + videoid + '_description').slideDown(400);
    //});
    //$('#' + videoid).mouseleave(function () {
    //    $('#' + videoid + '_description').slideUp(400);
    //});
    //$('#' + videoid).each(function () {
    //    $(this).qtip({
    //        content: description,
    //        position: {
    //            container: '.videolink_title',
    //            my: 'top middle',  // Position my top left...
    //            at: 'bottom middle', // at the bottom right of...
    //            adjust: {
    //                x: 0,
    //                y: 10
    //            }
    //        },
    //        style: {
    //            width: videowidth
    //        }
    //    });
    //});
    //$('#description').html('<b>Description</b>: ' + description);
    //$('#extrainfo').html('<b>Author</b>: ' + author + '<br/><b>Views</b>: ' + viewcount);
    //getComments(data.entry.gd$comments.gd$feedLink.href + '&max-results=50&alt=json', 1);
}

function parseVideoPlayerResults(data, element, videoid) {
    var video = {
        title: data.items[0].snippet.title,
        description: data.items[0].snippet.description,
        id: videoid
    };
    $(element).html('<div class="ytv-video"><div id="ytv-video-player"></div></div>');
    var description = document.createElement('div');
    description.innerHTML = '<div class="ytv-video-description" id="videodescription"></div>';
    var playerid = element.substring(1, element.length);
    var playerElement = document.getElementById(playerid);
    playerElement.parentNode.insertBefore(description, playerElement.nextSibling)
    var player = new YT.Player('ytv-video-player', {
        videoId: videoid,
        events: {
            onReady: function (e) {
                e.target.setPlaybackQuality('hd1080');
            },
            onStateChange: function (e) { }
        },
        playerVars: {
            autohide: 1,
            modestbranding: 1,
            enablejsapi: 1,
            origin: document.domain,
            controls: 1,
            rel: 0,
            showinfo: 0,
            iv_load_policy: 1,
            autoplay: 0,
            wmode: 'opaque'
        }
    });
    $('title').html((video.title));
    $('h1').html((video.title));
    $('#crumbs_page_title').html((video.title));
    $('.ytv-video-description').html(replaceLineBreaks(replaceURL(video.description)));
}

function parsePlaylistResults(data, videoid) {
    var title = data.items[0].title.$t;
    //var description = data.entry.media$group.media$description.$t;
    //var viewcount = data.entry.yt$statistics.viewCount;
    var author = data.feed.author[0].name.$t;
    $('DIV[name="' + videoid + '_title"]').each(function () {
        $(this).html(title);
    });
}

var playlists = new Array();
var videos = new Array();
var bParsed = false;
var categories = new Array();
function parseChannelGallery(data) {
    if (!bParsed) {
        var playlists = data.items;

        var bResults = false;
        if (params["category"]) {
            var title = 'Video Tutorials: ' + params["category"];
            $('#crumbs_page_title').html('<strong><a href="../Videos/index.html">Video Tutorials</a></strong> > ' + params["category"]);
            $('H1').html(title);
            $('.link_list_title').html('Series');
            var categoryPlaylists = new Array();
            for (i = 0; i < playlists.length; i++) {
                var playlistCategories = new Array();
                if (playlists[i].snippet.description.indexOf("Categories:") != -1) {
                    var catString = playlists[i].snippet.description.substring(playlists[i].snippet.description.indexOf("Categories:") + 11, playlists[i].snippet.description.length);
                    if (catString.charAt(catString.length - 1) == '.') {
                        catString = catString.slice(0, -1);
                    }
                    playlistCategories = catString.split(',');
                    for (j = 0; j < playlistCategories.length; j++) {
                        playlistCategories[j] = playlistCategories[j].trim();
                    }
                    if (playlistCategories.indexOf(params["category"]) != -1) {
                        $('#playlists').html($('#playlists').html() + constructPlaylistItem(playlists[i]));
                        categoryPlaylists.push(playlists[i]);
                        bResults = true;
                    }
                }
            }
            $('#loader').hide();
            if (!bResults) {
                $('#message').html("No Matching Playlists").show();
            }

            if (bResults && data.prevPageToken) {
                $('#loader_previous').click(function () { location.href = (location.href.indexOf('?') != -1 ? location.href.substring(0, location.href.indexOf('?')) : location.href) + "?category=" + params["category"] + "&page=" + data.prevPageToken; });
                $('#loader_previous').removeClass('disabled');
            }
            else {
                $('#loader_previous').click(function () { });
                $('#loader_previous').addClass('disabled');
            }

            if (bResults && data.nextPageToken) {
                $('#loader_next').click(function () { location.href = (location.href.indexOf('?') != -1 ? location.href.substring(0, location.href.indexOf('?')) : location.href) + "?category=" + params["category"] + "&page=" + data.nextPageToken; });
                $('#loader_next').removeClass('disabled');
            }
            else {
                $('#loader_previous').click(function () { });
                $('#loader_next').addClass('disabled');
            }
            if (bResults) {
                var rand = Math.floor(Math.random() * (categoryPlaylists.length - 1));
                $('.featured_series').html(constructCategoryImage(categoryPlaylists[rand]));
            }
        }
        else {
            var series = new Array();
            $('.link_list_title').html('Categories');
            for (i = 0; i < playlists.length; i++) {
                var playlistCategories = new Array();
                if (playlists[i].snippet.description.indexOf("Categories:") != -1) {
                    var catString = playlists[i].snippet.description.substring(playlists[i].snippet.description.indexOf("Categories:") + 11, playlists[i].snippet.description.length);
                    if (catString.charAt(catString.length - 1) == '.') {
                        catString = catString.slice(0, -1);
                    }
                    playlistCategories = catString.split(',');
                    for (j = 0; j < playlistCategories.length; j++) {
                        if (categories.indexOf(playlistCategories[j].trim()) == -1) {
                            categories.push(playlistCategories[j].trim());
                            series[playlistCategories[j].trim()] = 1;
                            bResults = true;
                        }
                        else {
                            series[playlistCategories[j].trim()] += 1;
                        }
                    }
                }
            }
            $('#loader').hide();
            if (!bResults) {
                $('#message').html("No Categories Available").show();
            }
            else {
                categories.sort();
                for (i = 0; i < categories.length; i++) {
                    $('#playlists').html($('#playlists').html() + constructPlaylistCategory(categories[i], series[categories[i]]));
                }
            }
            var rand = Math.floor(Math.random() * 12);
            $('.featured_series').html(constructFeaturedPlaylist(playlists[rand]));
        }

        bParsed = true;
    }
}

var bFeaturedParsed = false;
function parseFeaturedSeries(data) {
    if (!bFeaturedParsed) {
        var playlists = data.items;
        var rand = Math.floor(Math.random() * 12);
        $('.featured_series').html(constructFeaturedPlaylist(playlists[rand]));
    }
    bFeaturedParsed - true;
}

function constructPlaylistCategory(category, num) {
    var output = "";
    output += '<div name="' + category + '" class="playlistlink_container"">';
    output += '  <a class="playlistlink" href="../Videos/index.html?category=' + category + '">';
    output += '      <div class="playlistlink_title" style="margin-right:1em;padding-bottom:4px;float:none;">';
    output += '          <div style="float:right;color:#005990;">' + num + ' Series</div>';
    output += '          <div class="playlistlink_title_text" name="' + category + '_title">' + category + '</div>';
    output += '      </div>';
    output += '  </a>';
    output += '</div>';

    return output;
}

function constructCategoryImage(playlist) {
    var output = "";
    output += '<div class="playlistlink_container">';
    output += '   <div class="playlistlink">';
    output += '      <div class="playlistlink_image">';
    var thumb = playlist.snippet.thumbnails.high.url.replace("hqdefault", "maxresdefault").replace("http:", "https:");
    output += '          <img src="' + thumb + '" />';
    output += '      </div>';
    output += '   </div>';
    output += '</div>';

    return output;
}

function constructFeaturedPlaylist(playlist) {
    var output = "";
    output += '<div name="' + playlist.id.playlistId + '" class="playlistlink_container"">';
    output += '  <a class="playlistlink" href="../Videos/Player/index.html?series=' + playlist.id.playlistId + '">';
    output += '      <div class="playlistlink_image">';
    var thumb = playlist.snippet.thumbnails.high.url.replace("hqdefault", "maxresdefault");
    output += '          <img src="' + thumb + '" />';
    output += '      </div>';
    output += '      <div class="playlistlink_title">';
    output += '          <span class="playlistlink_title_label">FEATURED:</span>';
    output += '          <div class="playlistlink_title_text" name="' + playlist.id.playlistId + '_title">';
    output += playlist.snippet.title;
    output += '          </div>';
    output += '      </div>';
    output += '  </a>';
    output += '</div>';

    return output;
}

function constructPlaylistItem(playlist) {
    var output = "";
    output += '<div name="' + playlist.id.playlistId + '" class="playlistlink_container"">';
    output += '  <a class="playlistlink" href="../Videos/Player/index.html?series=' + playlist.id.playlistId + '">';
    output += '      <div class="playlistlink_image">';
    output += '          <img src="' + playlist.snippet.thumbnails.high.url + '" />';
    output += '      </div>';
    output += '      <div class="playlistlink_title">';
    output += '          <div class="playlistlink_title_text" name="' + playlist.id.playlistId + '_title">' + playlist.snippet.title + '</div>';
    output += '      </div>';
    output += '  </a>';
    output += '</div>';

    return output;
}

var bScrolling = new Array();
function setupDocPlaylistNav(params) {
    $('.track').each(function () {
        var listId = $(this).attr('id');
        $(this).find('A').each(function () {
            $(this).attr('href', $(this).attr('href') + '?track=' + listId);
        });
    });
    if (params["track"]) {
        $('DIV[name=' + params["track"] + ']').each(function () {
            $(this).find('A').each(function () {
                $(this).attr('href', $(this).attr('href') + '?track=' + params["track"]);
            });
            $(this).show();
        });
        if ($('.home').find('.menu_container').length > 0) {
            $('.home').addClass("has_menu");
        }
        $('.home').find('.menu_container').slimScroll({
            color: '#005990',
            opacity: 0.6,
            wrapperClass: "menu_scroll",
            barClass: 'menu_scroll_Bar',
            railClass: 'menu_scroll_rail',
            position: 'right',
            distance: 3,
            size: 9,
            width: '528px',
            height: '490px',
            railVisible: true,
            alwaysVisible: true,
            wheelStep: 5
        });
        bScrolling["menu_scroll_Bar"] = false;
        $('.menu_scroll_Bar').mousedown(function () { bScrolling["menu_scroll_Bar"] = true; });
        $(document).mouseup(function () { bScrolling["menu_scroll_Bar"] = false; });
        $('.home').children('.menu').hide();
        $('.home').mouseenter(function () { $(this).children('.menu').show(); });
        $('.home').mouseleave(function () { if (!bScrolling["menu_scroll_Bar"]) { $(this).children('.menu').hide(); } });
        $('.home').children('.menu').find('A').each(function () {
            var linkText = $(this).text();
            var titleText = $('H1').text();
            if (linkText == titleText) {
                $(this).replaceWith(linkText);
            }
        });
    }
}

String.prototype.toTitleCase = function () {
    var i, j, str, lowers, uppers;
    str = this.replace(/([^\W_]+[^\s-]*) */g, function (txt) {
        return txt.charAt(0).toUpperCase() + txt.substr(1).toLowerCase();
    });

    // Certain minor words should be left lowercase unless 
    // they are the first or last words in the string
    lowers = ['A', 'An', 'The', 'And', 'But', 'Or', 'For', 'Nor', 'As', 'At',
    'By', 'For', 'From', 'In', 'Into', 'Near', 'Of', 'On', 'Onto', 'To', 'With'];
    for (i = 0, j = lowers.length; i < j; i++)
        str = str.replace(new RegExp('\\s' + lowers[i] + '\\s', 'g'),
          function (txt) {
              return txt.toLowerCase();
          });

    // Certain words such as initialisms or acronyms should be left uppercase
    uppers = ['Id', 'Tv'];
    for (i = 0, j = uppers.length; i < j; i++)
        str = str.replace(new RegExp('\\b' + uppers[i] + '\\b', 'g'),
          uppers[i].toUpperCase());

    return str;
}

function replaceURL(text) {
    var exp = /(\b(https?|ftp|file):\/\/[-A-Z0-9+&@#\/%?=~_|!:,.;]*[-A-Z0-9+&@#\/%=~_|])/ig;
    return text.replace(exp, "<a href='$1'>$1</a>");
}

function replaceLineBreaks(text) {
    return '<p>' + text.replace(/\n([ \t]*\n)+/g, '</p><p>').replace('\n', '<br />') + '</p>';
}

function copyToClipboard(text) {
    if ($('#snippet_container').length == 0)
    {
        $('#maincol').append('<div id="snippet_container"><div id="snippet_close" onclick="$(\'#snippet_container\').remove();">X</div><textarea id="snippet_text" readonly="readonly" autofocus="autofocus">' + text + '</textarea><div style="margin-left:20px;">Press <strong>Ctrl + C</strong> to copy; <strong>Ctrl + V</strong> in graph to paste</div></div>');
        $('#snippet_text').select();
    }
}

var filters = {skillLevels : [], versions : [], tags: []};

function clearFilters()
{
	filters = {skillLevels : [], versions : [], tags: []};
	$('.skill_tag_filter').each(function(){
		if($(this).hasClass('enabled'))
		{
			$(this).removeClass('enabled');
		}
	});
	$('.version_tag_filter').each(function(){
		if($(this).hasClass('enabled'))
		{
			$(this).removeClass('enabled');
		}
	});
	updateFilters();
}

function addSkillFilter(filter)
{
	var skillFound = false;
	for(s=0; s < filters.skillLevels.length; s++)
	{
		if(filters.skillLevels[s].label == filter)
		{
			filters.skillLevels[s].isEnabled = true;
			$('.skill_tag_filter.' + filter).each(function(){
				if(!$(this).hasClass('enabled'))
				{
					$(this).addClass('enabled');
				}
			});
			skillFound = true;
		}
	}
	if(!skillFound)
	{
		filters.skillLevels.push({label: filter, isEnabled : true});
		$('.skill_tag_filter.' + filter).each(function(){
			$(this).addClass('enabled');
		});
	}
	updateFilters();
}

function removeSkillFilter(filter)
{
	var skillFound = false;
	for(s=0; s < filters.skillLevels.length; s++)
	{
		if(filters.skillLevels[s].label == filter)
		{
			filters.skillLevels[s].isEnabled = false;
			$('.skill_tag_filter.' + filter).each(function(){
				if($(this).hasClass('enabled'))
				{
					$(this).removeClass('enabled');
				}
			});
			skillFound = true;
		}
	}
	if(!skillFound)
	{
		$('.skill_tag_filter.' + filter).each(function(){
			$(this).removeClass('enabled');
		});
	}
	updateFilters();
}

function toggleSkillFilter(filter)
{
	var skillFound = false;
	for(s=0; s < filters.skillLevels.length; s++)
	{
		if(filters.skillLevels[s].label == filter)
		{
			filters.skillLevels[s].isEnabled = !filters.skillLevels[s].isEnabled;
			$('.skill_tag_filter.' + filter).each(function(){
				if($(this).hasClass('enabled'))
				{
					$(this).removeClass('enabled');
				}
				else
				{
					$(this).addClass('enabled');
				}
			});
			skillFound = true;
		}
	}
	if(!skillFound)
	{
		filters.skillLevels.push({label: filter, isEnabled : true});
		$('.skill_tag_filter.' + filter).each(function(){
			$(this).addClass('enabled');
		});
	}
	updateFilters();
}

function addVersionFilter(filter)
{
	var versionFound = false;
	for(s=0; s < filters.versions.length; s++)
	{
		if(filters.versions[s].label == filter)
		{
			filters.versions[s].isEnabled = true;
			$('.version_tag_filter.' + filter).each(function(){
				if(!$(this).hasClass('enabled'))
				{
					$(this).addClass('enabled');
				}
			});
			versionFound = true;
		}
	}
	if(!versionFound)
	{
		filters.versions.push({label: filter, isEnabled : true});
		$('.version_tag_filter.' + filter).each(function(){
			$(this).addClass('enabled');
		});
	}
	updateFilters();
}

function removeVersionFilter(filter)
{
	var versionFound = false;
	for(s=0; s < filters.versions.length; s++)
	{
		if(filters.versions[s].label == filter)
		{
			filters.versions[s].isEnabled = false;
			$('.version_tag_filter.' + filter).each(function(){
				if($(this).hasClass('enabled'))
				{
					$(this).removeClass('enabled');
				}
			});
			versionFound = true;
		}
	}
	if(!versionFound)
	{
		$('.version_tag_filter.' + filter).each(function(){
			$(this).removeClass('enabled');
		});
	}
	updateFilters();
}

function toggleVersionFilter(filter)
{
	var versionFound = false;
	for(s=0; s < filters.versions.length; s++)
	{
		if(filters.versions[s].label == filter)
		{
			filters.versions[s].isEnabled = !filters.versions[s].isEnabled;
			$('.version_tag_filter.' + filter).each(function(){
				if($(this).hasClass('enabled'))
				{
					$(this).removeClass('enabled');
				}
				else
				{
					$(this).addClass('enabled');
				}
			});
			versionFound = true;
		}
	}
	if(!versionFound)
	{
		filters.versions.push({label: filter, isEnabled : true});
		$('.version_tag_filter.' + filter).each(function(){
			$(this).addClass('enabled');
		});
	}
	updateFilters();
}

function toggleTag(tag)
{
	var tagFound = false;
	for(s=0; s < filters.tags.length; s++)
	{
		if(filters.tags[s].label == tag)
		{
			filters.tags[s].isEnabled = !filters.tags[s].isEnabled;
			$('.keyword_tag_filter.' + tag).each(function(){
				if($(this).hasClass('enabled'))
				{
					$(this).removeClass('enabled');
				}
				else
				{
					$(this).addClass('enabled');
				}
			});
			tagFound = true;
		}
	}
	if(!tagFound)
	{
		filters.tags.push({label: tag, isEnabled : true});
		$('.keyword_tag_filter.' + tag).each(function(){
			$(this).addClass('enabled');
		});
	}
	updateFilters();
}

function addTag(tag)
{
	var tagFound = false;
	for(s=0; s < filters.tags.length; s++)
	{
		if(filters.tags[s].label == tag)
		{
			filters.tags[s].isEnabled = true;
			$('.keyword_tag_filter.' + tag).each(function(){
				if(!$(this).hasClass('enabled'))
				{
					$(this).addClass('enabled');
				}
			});
			tagFound = true;
		}
	}
	if(!tagFound)
	{
		filters.tags.push({label: tag, isEnabled : true});
		$('.keyword_tag_filter.' + tag).each(function(){
			$(this).addClass('enabled');
		});
	}
	updateFilters();
}

function removeTag(tag)
{
	var tagFound = false;
	for(s=0; s < filters.tags.length; s++)
	{
		if(filters.tags[s].label == tag)
		{
			filters.tags[s].isEnabled = false;
			$('.keyword_tag_filter.' + tag).each(function(){
				if($(this).hasClass('enabled'))
				{
					$(this).removeClass('enabled');
				}
			});
			tagFound = true;
		}
	}
	if(!tagFound)
	{
		$('.keyword_tag_filter.' + tag).each(function(){
			$(this).removeClass('enabled');
		});
	}
	updateFilters();
}

function updateFilters()
{
	var sitemap = document.getElementById('sitemap');
	if(sitemap != null)
	{
		var items = sitemap.getElementsByTagName('li');
		var isFiltered = false;
		for(i=0; i < items.length; i++)
		{
			CollapsibleLists.Close(items[i]);
		}
		var isFiltered = false;
		for(i=0; i < filters.skillLevels.length; i++)
		{
			if(filters.skillLevels[i].isEnabled)
			{
				isFiltered = true;
			}
		}
		for(i=0; i < filters.versions.length; i++)
		{
			if(filters.versions[i].isEnabled)
			{
				isFiltered = true;
			}
		}
		for(i=0; i < filters.tags.length; i++)
		{
			if(filters.tags[i].isEnabled)
			{
				isFiltered = true;
			}
		}
		
		if(!isFiltered)
		{
			$('.link').each(function(){
				$(this).removeClass('unfiltered');
				$(this).removeClass('skillfiltered');
				$(this).removeClass('versionfiltered');
				$(this).removeClass('tagfiltered');
			});
			//$('.tag_wrapper').hide();
			//$('.tag_container').html('');
		}
		else
		{
			$('.link').each(function(){
				$(this).addClass('unfiltered');
			});
			for(i=0; i < filters.skillLevels.length; i++)
			{
				if(filters.skillLevels[i].isEnabled)
				{
					$('.link.' + filters.skillLevels[i].label.toLowerCase()).each(function(){
						$(this).removeClass('unfiltered');
						$(this).addClass('skillfiltered');
						$(this).parents('li').each(function () {
							CollapsibleLists.Open($(this).get(0));
						});
					});
					isFiltered = true;
				}
				else
				{
					$('.link.' + filters.skillLevels[i].label.toLowerCase()).each(function(){			
						$(this).removeClass('skillfiltered');
						if(!$(this).hasClass('versionfiltered') && !$(this).hasClass('tagfiltered'))
						{
							$(this).addClass('unfiltered');
						}
					});
				}
			}
			
			for(j=0; j < filters.versions.length; j++)
			{
				if(filters.versions[j].isEnabled)
				{
					$('.link.version' + filters.versions[j].label.toLowerCase()).each(function(){
						$(this).removeClass('unfiltered');
						$(this).addClass('versionfiltered');
						$(this).parents('li').each(function () {
							CollapsibleLists.Open($(this).get(0));
						});
					});
					isFiltered = true;
				}
				else
				{
					$('.link.version' + filters.versions[j].label.toLowerCase()).each(function(){
						$(this).removeClass('versionfiltered');
						if(!$(this).hasClass('skillfiltered') && !$(this).hasClass('tagfiltered'))
						{
							$(this).addClass('unfiltered');
						}
					});
				}
			}
			
			for(j=0; j < filters.tags.length; j++)
			{
				if(filters.tags[j].isEnabled)
				{
					$('.link.' + filters.tags[j].label.toLowerCase()).each(function(){
						$(this).removeClass('unfiltered');
						$(this).addClass('tagfiltered');
						$(this).parents('li').each(function () {
							CollapsibleLists.Open($(this).get(0));
						});
					});
					isFiltered = true;
				}
				else
				{
					$('.link.' + filters.tags[j].label.toLowerCase()).each(function(){
						$(this).removeClass('tagfiltered');
						if(!$(this).hasClass('skillfiltered') && !$(this).hasClass('versionfiltered'))
						{
							$(this).addClass('unfiltered');
						}
					});
				}
			}
		}
	}
}

function changeLanguage(lang)
{
	var href = document.location.href;
	 href = href.replace(CurrLang, lang);
	 if(href.lastIndexOf('/') == href.length - 1)
	 {
		href = href + 'index.html';
	 }
	 document.location.href = href;
}

function CheckInternal(href)
{
    var regExp = new RegExp("//" + location.host + "($|/)");
	if(	href.indexOf(".jpg") || 
		href.indexOf(".png") || 
		href.indexOf(".gif") || 
		href.indexOf(".mp4") || 
		href.indexOf("SiteIndex/index.html") != -1 || 
		href.indexOf("/API/") != -1 || 
		href.indexOf("/BlueprintAPI/") != -1 || 
		href.substring(0, 4) === "http" ? !regExp.test(href) : false )
	{
		return false;
	}
	else
	{
		return true;
	}
}