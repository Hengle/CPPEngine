var margin;
var state = "open";

$(document).ready(function(){
	//ContentFix
	$("#navigation").html("<");
	getSidebarState();
	if(state != "closed")
	{
		Open();
		$("#navigation").click(SlideIn);
	}
	else
	{
		Close();
		$("#navigation").click(SlideOut);
	}
});

function setSidebarState(newState) {
    eraseCookie("doc_Sidebar_State");
    createCookie("doc_Sidebar_State", newState, 30);
}

function getSidebarState() {
    state = readCookie("doc_Sidebar_State");
}

//******************** NAVIGATION PANEL FUNCTIONS *********************//
// Slides the panel into and out of view and fixes the margins and
// positions of the inner elements so that everything remains uniform
function SlideIn(){
	//get DOM elements
	var navPanel = document.getElementById("navPanel");
	var navigationButton = document.getElementById("navigation");
	var navigationWrapper = document.getElementById("navWrapper");
	var pageDefault = document.getElementById("contentContainer");	
	
	//set transitions
	navPanel.style.transition = "left 0.5s linear 0s";
	navigationButton.style.transition = "left 0.5s linear 0s";
	navigationWrapper.style.transition = "width 0.5s linear 0s";
	pageDefault.style.transition = "margin-left 0.5s linear 0s";
	
	//Perform page transitions to hide navBar
	navPanel.style.left = "-300px";
	navigationButton.style.left = "-8px";
	navigationWrapper.style.width = "0px";
	pageDefault.style.marginLeft = "0px";
	
	//ContentFix
	$("#navigation").html(">");
	
	//attach new event handlers
	$("#navigation").off("click");
	$("#navigation").on("click", SlideOut);
	setSidebarState("closed");
}

function SlideOut(){
	//get DOM elements
	var navPanel = document.getElementById("navPanel");
	var navigationButton = document.getElementById("navigation");
	var navigationWrapper = document.getElementById("navWrapper");
	var pageDefault = document.getElementById("contentContainer");	
	
	//set transitions
	navPanel.style.transition = "left 0.5s linear 0s";
	navigationButton.style.transition = "left 0.5s linear 0s";
	navigationWrapper.style.transition = "width 0.5s linear 0s";
	pageDefault.style.transition = "margin-left 0.5s linear 0s";
	
	//Perform page transitions to hide navBar
	navPanel.style.left = "0px";
	navigationButton.style.left = "292px";
	navigationWrapper.style.width = "300px";
	pageDefault.style.marginLeft = "300px";
	
	//ContentFix
	$("#navigation").html("<");
	
	//attach new event handlers
	$("#navigation").off("click");
	$("#navigation").on("click", SlideIn);
	setSidebarState("open");
}

function Close(){
	//get DOM elements
	var navPanel = document.getElementById("navPanel");
	var navigationButton = document.getElementById("navigation");
	var navigationWrapper = document.getElementById("navWrapper");
	var pageDefault = document.getElementById("contentContainer");	
	
	//set transitions
	navPanel.style.transition = "left 0s linear 0s";
	navigationButton.style.transition = "left 0s linear 0s";
	navigationWrapper.style.transition = "width 0s linear 0s";
	pageDefault.style.transition = "margin-left 0s linear 0s";
	
	//Perform page transitions to hide navBar
	navPanel.style.left = "-300px";
	navigationButton.style.left = "-8px";
	navigationWrapper.style.width = "0px";
	pageDefault.style.marginLeft = "0px";
	
	//ContentFix
	$("#navigation").html(">");
	
	//attach new event handlers
	$("#navigation").off("click");
	$("#navigation").on("click", SlideOut);
	setSidebarState("closed");
}

function Open(){
	//get DOM elements
	var navPanel = document.getElementById("navPanel");
	var navigationButton = document.getElementById("navigation");
	var navigationWrapper = document.getElementById("navWrapper");
	var pageDefault = document.getElementById("contentContainer");	
	
	//set transitions
	navPanel.style.transition = "left 0s linear 0s";
	navigationButton.style.transition = "left 0s linear 0s";
	navigationWrapper.style.transition = "width 0s linear 0s";
	pageDefault.style.transition = "margin-left 0s linear 0s";
	
	//Perform page transitions to hide navBar
	navPanel.style.left = "0px";
	navigationButton.style.left = "292px";
	navigationWrapper.style.width = "300px";
	pageDefault.style.marginLeft = "300px";
	
	//ContentFix
	$("#navigation").html("<");
	
	//attach new event handlers
	$("#navigation").off("click");
	$("#navigation").on("click", SlideIn);
	setSidebarState("open");
}
//
//***************** END NAVIGATION PANEL FUNCTIONS *********************//