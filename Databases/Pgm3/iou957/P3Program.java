package cs3743;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Date;

public class P3Program 
{
    private Connection connect = null;
    
    private Statement statement = null;
    private PreparedStatement preparedStatement = null;
    private ResultSet resultSet = null;
    public static final int ER_DUP_ENTRY = 1062;
    public static final int ER_DUP_ENTRY_WITH_KEY_NAME = 1586;
    public static final String[] strFlightIdM =
    {   "510"
       ,"705"
       ,"331"
       ,"END"
    };
    
    public P3Program (String user, String password) throws Exception
    {
        try
        {
            // This will load the MySQL driver, each DBMS has its own driver
            Class.forName("com.mysql.jdbc.Driver");
            this.connect = DriverManager.getConnection
                    ("jdbc:mysql://10.100.1.81:3306/iou957db"
                    , user
                    , password);
        }
        catch (Exception e) 
        {
            throw e;
        } 
        
    }
    
    public void runProgram() throws Exception 
    {
    	int i; //for strFlightIdM array
    	int reqSeatQty = 1;
        try 
        {
        	statement = connect.createStatement();
        	//a.	Use statement to execute this select statement
        	resultSet = statement.executeQuery("select c.* from Customer c");
        	//b.	Use printCustomers("Beginning Customers", resultSet) to print those customers.
        	printCustomers("Beginning Customers", resultSet);
        	
        	//c.	Use statement to execute this select statement:
        	resultSet = statement.executeQuery("select f.* from Flight f");
        	//d.	Use MySqlUtility.printUtility("Beginning Flights", resultSet) to print those flights
        	MySqlUtility.printUtility("Beginning Flights", resultSet);
        	
        	//e.	Use statement.executeUpdate to insert a row into Customer 
        	try { //Nested try catch unique for execute update incase of dups
        	statement.executeUpdate("insert into iou957db.Customer " 
    				+ "(`custNr` , `name` , `preferAirline` , `birthDt` , `gender`)"
    				+ "values(\"1999\" , \"Darin Soeung\" , \"PoDunk\" , \"1998-03-20\" , \"M\")");
        	}
        	catch (SQLException e)
        	{
        	    switch (e.getErrorCode())
        	    {
        	        case ER_DUP_ENTRY:
        	        case ER_DUP_ENTRY_WITH_KEY_NAME:
        	            System.out.printf("Duplicate key error: %s\n", e.getMessage());
        	            break;
        	        default:
        	            throw e;
        	    }
        	}
        	//f.	Use statement to get the customers (as was done in step a.) 
        	resultSet = statement.executeQuery("select * from iou957db.Customer");
        	//and printCustomers("Customers after I was added", resultSet).
			printCustomers("Customers after I was added", resultSet);
			//g.	Print the index information from the MYSQL catalog for the Reservation table
					//Use statement to get the index column information for this query:
					//hint break it into multiple literal pieces and concatenate them together
			resultSet = statement.executeQuery("select TABLE_SCHEMA, TABLE_NAME, INDEX_NAME, SEQ_IN_INDEX, COLUMN_NAME, CARDINALITY "
					+ "from INFORMATION_SCHEMA.STATISTICS "
					+ "where TABLE_SCHEMA = \"iou957db\" "
					+ "and TABLE_NAME = \"Reservation\" "
					+ "order by INDEX_NAME, SEQ_IN_INDEX");
			MySqlUtility.printUtility("My Reservation Indexes", resultSet);
			//h.	Create a prepareStatement which does an insert into Reservation using subsitution parameters for each attribute.
					//This should be done outside the for statement of the next step.
			preparedStatement = connect.prepareStatement( "Insert into  iou957db.Reservation "
					//Show sub parameters as question marks
					+ "values (?,?,?)");
			//i.	Use a Java for statement to iterate through the strFlightIdM(above) array until a value of "END" is encountered.  
			for(i=0; i < strFlightIdM.length; i++)
			{
				//if "END" is encountered should be last
				if(strFlightIdM[i] == "END")
					break;
				//SET NOTATION, setType(parmNr, value)
				//Reservation(int custNr, str flightId, int reqSeatQty);
				//Set the substitution parameters as follows:
				//	Parameter 1 to 1999.
				preparedStatement.setInt(1, 1999);
				//	Parameter 2 to strFlightIdM[i].
				preparedStatement.setString(2, strFlightIdM[i]);
				//	Parameter 3 to reqSeatQty value which is 1 for the first reservation.
				//  Increase that by 1 for each of the reservations (i.e, use 1, 2, 3).
				//	seats initialized as 1
				preparedStatement.setInt(3, reqSeatQty);
				//	increased for each reservation
				reqSeatQty++;
				//Execute the insertion using preparedStatement.executeUpdate().
					//using another try catch similar to the one above also with executeUpdate()
				try { //Nested try catch unique for execute update incase of dups
					preparedStatement.executeUpdate();
		        	}
		        catch (SQLException e)
		        	{
		        		switch (e.getErrorCode())
		        	    {
		        	        case ER_DUP_ENTRY:
		        	        case ER_DUP_ENTRY_WITH_KEY_NAME:
		        	            System.out.printf("Duplicate key error: %s\n", e.getMessage());
		        	            break;
		        	        default:
		        	            throw e;
		        	    }
		        	}
			}
			//j.	Create a prepareStatement to select reservations (all columns) for a custNr that is provided as a subsitution parameter.
			preparedStatement = connect.prepareStatement("SELECT * from iou957db.Reservation where custNr = ?");
			//Provide 1999 for the custNr parameter and use preparedStatement.executeQuery() to execute it.  
			preparedStatement.setInt(1, 1999);
			resultSet = preparedStatement.executeQuery();
			//Print the result using MySqlUtility.printUtility("My reservations", resultSet).
			MySqlUtility.printUtility("My reservations", resultSet);
			//k.	Use statement to execute a select statement which returns 
					//the flightId, custNr, customer name, and reqSeatQty for customers who are on flights that customer 1999 is on.  
					//Do NOT include 1999 in the returned rows.  Print them using 
			preparedStatement = connect.prepareStatement("select r.flightId, r.custNr, c.name, r.reqSeatQty "
					+ "from Customer c, Reservation r "
					+ "where c.custNr <> 1999 and exists ( "
						+ "select * "
						+ "from Reservation r, Reservation r1999 "
						+ "where r1999.custNr = 1999 " 
						+ "and r1999.flightId = r.flightId "
						+ "and c.custNr = r.custNr ) "
						+ "and c.custNr = r.custNr ");
			resultSet = preparedStatement.executeQuery();
			
			MySqlUtility.printUtility("Other customers on my flights", resultSet);
			//l.	Use statement.executeUpdate to update the reservations for custNr 1999 by doubling them.
					//(i.e., set reqSeatQty = reqSeatQty * 2).  
			statement.executeUpdate("update Reservation set reqSeatQty = (reqSeatQty * 2) where custNr = 1999 ");
			//m.	Use statement to execute this select statement:
					//select r.* from Reservation r
			resultSet = statement.executeQuery("select r.* from Reservation r");
			//n.	Use MySqlUtility.printUtility("Modified Reservations", resultSet) to print those flights.
			MySqlUtility.printUtility("Modified Reservations", resultSet);
			//o.	Use statement to execute a select statement which returns flightId and count(*) for flights having more than two reservations.  
					//(See example #16 in the SQL DML notes.) uses COUNT(e.final) to count elements  
			resultSet = statement.executeQuery("select r.flightId, COUNT(r.flightId) "
					+ "from iou957db.Reservation r "
					+ "group by r.flightId "
					+ "having COUNT(r.flightId)>1");
			//Print the results using MySqlUtility.printUtility("Flights Having more than 2 reservations", resultSet).
			MySqlUtility.printUtility("Flights Having more than 2 reservations", resultSet);
			//p.	Delete the reservations for customer 1999 from the Reservation Table using statement
			statement.executeUpdate("delete from iou957db.Reservation where custNr = 1999");
			//q.	Reusing the preparedStatement from step j, again provide 1999 for the custNr parameter and 
					//use preparedStatement.executeQuery() to execute it.  Print the result using MySqlUtility.printUtility 
					//("My Reservations after Deleting", resultSet).
			resultSet = statement.executeQuery("select * from iou957db.Reservation where custNr = 1999");
			MySqlUtility.printUtility("My Reservations after Deleting", resultSet);
			//r.	Repeat step k, but use this for the title "Other customers on my flights after mine were deleted".
					//Of course, no results are printed.
			preparedStatement = connect.prepareStatement("select r.flightId, r.custNr, c.name, r.reqSeatQty "
					+ "from Customer c, Reservation r "
					+ "where c.custNr <> 1999 and exists ( "
						+ "select * "
						+ "from Reservation r, Reservation r1999 "
						+ "where r1999.custNr = 1999 " 
						+ "and r1999.flightId = r.flightId "
						+ "and c.custNr = r.custNr ) "
						+ "and c.custNr = r.custNr ");
			resultSet = preparedStatement.executeQuery();
			MySqlUtility.printUtility("Other customers on my flights after mine were deleted", resultSet);
        } 
        catch (Exception e) 
        {
            throw e;
        } 
        finally 
        {
            close();
        }

    }                                                                                                                        
    
    private void printCustomers(String title, ResultSet resultSet) throws SQLException 
    {
        // Your output for this must match the format of my sample output exactly. 
        // custNr, name, preferAirline, birthDt, gender
        System.out.printf("%s\n", title);
        // your code
        //however, you should indent the column headings and column data four spaces to improve readability. 
        //left justifying
        System.out.printf("%-5s    %-20s    %-15s    %-11s    %-4s\n" , "custNr", "name", "preferAirline", "birthDt", "gender");
        //Hint use printSections 
        // print each tuple in the result set
        while (resultSet.next())
		{
			String custStr = resultSet.getString("custNr"); 	//can't be null
			String nameStr = resultSet.getString("name");		//can be null
			String prefAirStr = resultSet.getString("preferAirline");	//can be null
			String birthDtStr = resultSet.getString("birthDt");		//can be null
			String genderStr = resultSet.getString("gender");		//honestly in this day and age it's a touchy subject
																	//in order to not offend anyone in reality I would put possibly null
																	//I hope that's ok
			
			// List  of possible nulls? I think name, birthdt for sure, preferred airline for sure
			// print "---" for null
			if (nameStr == null)
				nameStr = "---";
			if (prefAirStr == null)
				prefAirStr = "---";
			java.sql.Date birthDtC = resultSet.getDate("birthDt");
			if (birthDtC == null || birthDtStr == "null")
				birthDtStr = "---";
			else
				birthDtStr = birthDtC.toString().substring (0,10);
			if (genderStr == null)
				genderStr = "---";
			if (genderStr == null)
				genderStr = "---";
			
			//you should indent the column headings and column data four spaces to improve readability.  
			System.out.printf("%-5s    %-20s    %-15s    %-11s    %-4s\n" , custStr, nameStr, prefAirStr, birthDtC, genderStr);
		}
		System.out.printf("\n");

    }
    

    // Close the resultSet, statement, preparedStatement, and connect
    private void close() 
    {
        try 
        {
            if (resultSet != null) 
                resultSet.close();

            if (statement != null) 
                statement.close();
            
            if (preparedStatement != null) 
                preparedStatement.close();

            if (connect != null) 
                connect.close();
        } 
        catch (Exception e) 
        {

        }
    }

}
