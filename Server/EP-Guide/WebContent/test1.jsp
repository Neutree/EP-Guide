<html>
<head>
	<%@ page contentType="text/html; charset=utf-8" %>
</head>
<body>
	<form action="" id="form" method="post">
		Url:<input  type="text" id="url" /><br />
		Data:<input  type="text" name="data"><br />
		<button onclick="post()">确定</button>
	</form>
</body>
</html>
<script type="text/javascript">
	function post(){
		document.getElementById("form").action = document.getElementById('url').value;
		document.getElementById("form").submit();
	}

</script>