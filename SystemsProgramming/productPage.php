<?php
include_once('server_config.php');

$conn = new mysqli ($host, $dbusername, $dbpassword, $dbname);
$query = "SELECT * FROM Products ";
$result = (mysqli_query($conn, $query));
$conn -> close()
?>

<!DOCTYPE html>
<HTML>
<HEAD>
    <meta name="viewport" content="width=device-width, initial-scale=1"/>
    <link rel="stylesheet" type="text/css" href="styles.css">
    <script src="popup.js" type="text/javascript"></script>
</HEAD>

<BODY>

<script src="http://ajax.googleapis.com/ajax/libs/jquery/1.7.1/jquery.min.js" type="text/javascript"></script>

<!--Navigation bar-->
<div id="nav-placeholder">

</div>


<script>
    $(document).ready(function () {
        $("#nav-placeholder").load("nav.html");
    });
</script>


<table  name="cartTable" align="center" title="Your Cart">
    <tr class="head">
        <th colspan="5"><h2> Items in Cart</h2></th>
    </tr>

    <tr class="sub_head">
        <!--
             NOT FOR SHOPPER SIDE: <th> Product ID </th>
             NOT FOR SHOPPER SIDE:   <th> Product Quantity </th>
             NOT FOR CART VIEW: <th> Add to Cart</th>-->
        <th> Product SKU</th>
        <th> Product Name</th>
        <th class="money"> Product Price</th>
        <th> Product Expire Date</th>

    </tr>

    </tr>
</table>


</BODY>
</HTML>