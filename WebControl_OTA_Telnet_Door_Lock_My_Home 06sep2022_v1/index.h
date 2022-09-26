const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
   <style type="text/css">
      .button {
      background-color: #0319F7; /* BLUE */
      border: none;
      color: white;
      padding: 15px 32px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 16px;
      }
      /* Bordered form */
      form {
        border: 3px solid #f1f1f1;
      }

      /* Full-width inputs */
      input[type=text], input[type=password] {
        width: 100%;
        padding: 12px 20px;
        margin: 8px 0;
        display: inline-block;
        border: 1px solid #ccc;
        box-sizing: border-box;
      }

      /* Set a style for all buttons */
      button {
        background-color: #04AA6D;
        color: white;
        padding: 14px 20px;
        margin: 8px 0;
        border: none;
        cursor: pointer;
        width: 100%;
      }

      /* Add a hover effect for buttons */
      button:hover {
        opacity: 0.8;
      }

      /* Extra style for the cancel button (red) */
      .cancelbtn {
        width: auto;
        padding: 10px 18px;
        background-color: #f44336;
      }

      /* Center the avatar image inside this container */
      .imgcontainer {
        text-align: center;
        margin: 24px 0 12px 0;
      }

      /* Avatar image */
      img.avatar {
        width: 40%;
        border-radius: 50%;
      }

      /* Add padding to containers */
      .container {
        padding: 16px;
      }

      /* The "Forgot password" text */
      span.psw {
        float: right;
        padding-top: 16px;
      }

      /* Change styles for span and cancel button on extra small screens */
      @media screen and (max-width: 300px) {
        span.psw {
          display: block;
          float: none;
        }
        .cancelbtn {
          width: 100%;
        }
      }      
   </style>
   <title>Door Lock</title>
   <link rel="icon" href="http://erp.superhomebd.com/super_home/assets/img/favicon.png" type="image/gif" sizes="16x16">
   <center>
      <div>
         <img src="http://erp.superhomebd.com/super_home/assets/img/neways.png" style="width: 150px;padding-top: 15px;" alt="Super Hostel" title="Super Hostel">
         <h1>
            <marquee>Rasel Home Door Lock</marquee>
         </h1>
         <div id = "state">
         
         </div>
         <button class="button" onclick="send(1)">Door Open</button>
        
      </div>

<br>     
      <script>
         function send(lock_sts) {
           var xhttp = new XMLHttpRequest();
           xhttp.onreadystatechange = function() {
             if (this.readyState == 4 && this.status == 200) {
                 //document.getElementById("state").innerHTML = 
                 this.responseText;
//                 console.log(this.responseText);
             }
           };
           xhttp.open("GET", "product?state="+lock_sts, true);
           xhttp.send();
         }
         //setInterval(function() {
         //  getData();
         //}, 2000); 
      </script>
   </center>
   </body>
</html>
)=====";