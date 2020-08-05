<?php
include_once('server_config.php');

$conn = new mysqli ($host, $dbusername, $dbpassword, $dbname);
$query = "SELECT * FROM Products ";
$result = (mysqli_query($conn, $query));
$conn -> close()
?>

<?php
include_once('server_config.php');
//mysqli_report(MYSQLI_REPORT_ERROR | MYSQLI_REPORT_STRICT);
$conn = new mysqli ($host, $dbusername, $dbpassword, $dbname);

//WHERE ProductName IN ('Holly', 'Ryan')


$sql = "SELECT * FROM Products WHERE ProductName IN (";

//$value = $_POST['checkbox'];

foreach($_POST['checkbox'] as $value){
	$sql = $sql. "'"."$value"."',";
}

$sql = ")";

//foreach($_POST['check_list'] as $selected) {
//echo "<p>".$selected ."</p>";
//}

$cartList = (mysqli_query($conn, $sql));
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

<form id = "proceed_to_checkout" action = "checkoutPage.php" method="post">

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
    
    <?php
            setlocale(LC_MONETARY, 'en_US.UTF-8');

            $x = 0;
            while ($rows = mysqli_fetch_assoc($cartList)) {

            ?>

                <?php

                if ($x % 2 == 0) {
                    $x++; ?>

                    <tr class="even" >
                        <!-- TODO: implement to associate side

                               NOT FOR SHOPPER SIDE: <td><?php echo $rows['product_id']; ?></td>
                               NOT FOR SHOPPER SIDE: <td><?php echo $rows['ProductQuantity']; ?></td> -->

                        <!-- TODO: implement add to cart function-->
                        <td><input type="checkbox" name="checkbox" value=<?php echo $rows['ProductName']; ?> /> &nbsp;</td>
                        <td><?php echo $rows['ProductSKU']; ?></td>
                        <td>
                            <?php echo $rows['ProductName']; ?> <br />

                            <?php
                            $dir = "images/product_images/";
                            $compareValue = $rows['ProductSKU'];
                            if ($opendir = opendir($dir)) {
                                while (($file = readdir ($opendir)) !== FALSE) {
                                    if ($file!="."&&$file!="..") {
                                        if (strpos($file, $compareValue) !== FALSE) {
                                            $dir2 = "images/product_images/$file";
                                            if ($opendir2 = opendir($dir2)) {
                                                while (($file2 = readdir($opendir2)) !== FALSE) {
                                                    if ($file2!="."&&$file2!="..") {
                                                        echo "<img src='$dir2/$file2' width='100' height='100' >";
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            ?>

                        </td>
                        <td class="money"><?php echo money_format('%.2n', $rows['ProductPrice']); ?></td>
                        <td><?php echo $rows['ProductExpireDate']; ?></td>

                    </tr>
                <?php } else {
                    $x++;
                    ?>
                    <tr class="odd">
                        <!-- TODO: implement to associate side

                               NOT FOR SHOPPER SIDE: <td><?php echo $rows['product_id']; ?></td>
                               NOT FOR SHOPPER SIDE: <td><?php echo $rows['ProductQuantity']; ?></td> -->

                        <!-- TODO: implement add to cart function-->
                        <td><input type="checkbox" name="checkbox" value=<?php echo $rows['ProductName']; ?>/> &nbsp;</td>
                        <td><?php echo $rows['ProductSKU']; ?></td>
                        <td><?php echo $rows['ProductName']; ?> <br/>

                            <?php
                            $dir = "images/product_images/";
                            $compareValue = $rows['ProductSKU'];
                            if ($opendir = opendir($dir)) {
                                while (($file = readdir ($opendir)) !== FALSE) {
                                    if ($file!="."&&$file!="..") {
                                        if (strpos($file, $compareValue) !== FALSE) {
                                            $dir2 = "images/product_images/$file";
                                            if ($opendir2 = opendir($dir2)) {
                                                while (($file2 = readdir($opendir2)) !== FALSE) {
                                                    if ($file2!="."&&$file2!="..") {
                                                        echo "<img src='$dir2/$file2' width='100' height='100' >";
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            ?>
                        </td>
                        <td class="money"><?php echo money_format('%.2n', $rows['ProductPrice']); ?></td>
                        <td><?php echo $rows['ProductExpireDate']; ?></td>

                    </tr>
                <?php } ?>

            <?php } ?>

    </tr>
</table>

    <div class="foot" style="text-align:center">
        <button type="submit" value="proceed_to_checkout">Proceed to Checkout</button>
    </div>

</form>
</BODY>
</HTML>