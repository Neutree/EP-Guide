<%@ page contentType="text/html; charset=utf-8" %>
<% int parkID=2; %>
<html>
<head>
	<title>ParkAnimation</title>
	<style> 
	div
	{
		margin-top: 50%;
		margin-left: 50%;
		width:30px;
		height:45px;
		background:red;
		position:relative;
		animation-name:myfirst;
		animation-duration:5s;
		animation-timing-function:linear;
		animation-delay:2s;
		animation-iteration-count:20;
		animation-direction:alternate;
		animation-play-state:running;
		animation-fill-mode:both;
		/* Safari and Chrome: */
		-webkit-animation-name:myfirst;
		-webkit-animation-duration:3s;
		-webkit-animation-timing-function:linear;
		-webkit-animation-delay:2s;
		-webkit-animation-iteration-count:20;
		-webkit-animation-direction:alternate;
		-webkit-animation-play-state:running;
		-webkit-animation-fill-mode:both;
	}

	
	<% if (parkID == 1 ) { %>
     @keyframes myfirst/* Safari and Chrome */
	{
		0%   {background:red; left:=0px; top:0px;}
		50%  {background:yellow; left:0px; top:-200px;}
		100%  {background:blue; left:-200px; top:-200px;}
	}
	<% } 
	if (parkID == 2) { %>
	     @keyframes myfirst/* Safari and Chrome */
	{
		0%   {background:red; left:=0px; top:0px;}
		50%  {background:yellow; left:0px; top:-200px;}
		100%  {background:blue; left:200px; top:-200px;}
	
	}
	<% } %>
	<% if (parkID == 3 ) { %>
     @keyframes myfirst/* Safari and Chrome */
	{

		0%  {background:blue; left:-200px; top:-200px;}
		50%  {background:yellow; left:0px; top:-200px;}
		100%   {background:red; left:=0px; top:0px;}
	}
	<% } 
	if (parkID == 4) { %>
	     @keyframes myfirst/* Safari and Chrome */
	{
		0%  {background:blue; left:200px; top:-200px;}
		50%  {background:yellow; left:0px; top:-200px;}
		100%   {background:red; left:=0px; top:0px;}
	}
	<% } %>

	</style>
</head>
<body>

<div></div>
</body>
</html>