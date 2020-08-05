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
        <script src="http://ajax.googleapis.com/ajax/libs/jquery/1.7.1/jquery.min.js" type="text/javascript"></script>
    </HEAD>

    <BODY style="background-image: url('paper.jpg'); background-repeat: repeat;">

        <!--Navigation bar-->
        <div id="nav-placeholder"></div>

        <script>
            $(document).ready(function () {
                $("#nav-placeholder").load("nav.html");
            });
        </script>
        <center>
        <div class="overlay">
        <br/>
        <div style="position: relative;text-align: center;color: white;">
            <img src="grocerybanner.jpg" style="width: 70%; height: 70%;"/>
        </div>
        <br/>
        <form id = "add_to_cart" action = "user_cart.php" method="post">
        <table  class="productTable" align="center" >
            <tr class="head">
                <th colspan="5"><h2> Products</h2></th>
            </tr>

            <tr class="sub_head">
                <!--v
                     NOT FOR SHOPPER SIDE: <th> Product ID </th>
                     NOT FOR SHOPPER SIDE:   <th> Product Quantity </th> -->
                <th> Add to Cart</th>
                <th> Product SKU</th>
                <th> Product Name</th>
                <th class="money"> Product Price</th>
                <th> Product Expire Date</th>

            </tr>
            <?php
            setlocale(LC_MONETARY, 'en_US.UTF-8');

            $x = 0;
            while ($rows = mysqli_fetch_assoc($result)) {

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
            <button type="submit" value="add_to_cart">Add to Cart</button>
        </div><br/>

        </form>
        </div>
        </center>
    </BODY>
</HTML>