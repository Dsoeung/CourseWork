import java.lang.reflect.Field;
public class Reflect {
   private static class Stuff {
       public Integer a = 1;
       public Double b = 200.0;
       public Integer c = 3;
       public Double d = 400.0;
       public String m = "a";
       public String n = "xy";
	   
       public void prt() {
           System.out.println("[" + a + ", " + b + ", " + c + ", " + d + ", " + m + ", " + n + "]");
       }
	   
   }
   
   private void info(Object obj) {
       // TBD: Enter code here to display information about the object
       Field[] fields = Stuff.class.getFields();
	   //for fields in f print name and type
	   //example: ** Field = a is java.lang.Integer
       for (Field f : fields) {
           System.out.println("** Field = " + f.getName() + " " + f.getType());
       }
   }

   private void modify(Object obj) {
       try {
           // TBD: Enter code here to change the object
           Stuff sTemp = (Stuff) obj;
           Field[] fTemp = Stuff.class.getFields();
		   //Check value in field typing through else if()
		   // ints * 10, double + 10, string = string(^+1)
			   //example field : [1, 200.0, 3, 400.0, 'a', 'xy'] [10, 210.0, 30, 410.0, 'aa', 'xyxy'] [100, 220.0, 300, 420.0, 'aaaa', 'xyxyxyxy']
           for (Field fIter : fTemp) {
               Class typing = fIter.getType();
               Object value = fIter.get(sTemp);
               if (typing == Integer.class) {	
                   int num = (int) value;
				   // example field : [10, 210.0, 30, 410.0, aa, xyxy]
				   //typing "rule"
                   num = num * 10;
                   fIter.set(sTemp, num);
               } else if (typing == Double.class) {
                   double flNum = (double) value;
				   //example field : [10, 210.0, 30, 410.0, aa, xyxy]
				   //typing "rule"
                   flNum = flNum + 10.0;
                   fIter.set(sTemp, flNum);
               } else if (typing == String.class) {
                   String sVal = (String) value;
				   //example field : [10, 210.0, 30, 410.0, aa, xyxy]
				   //typing "rule"
                   sVal = sVal + sVal;
                   fIter.set(sTemp, sVal);
               }
           }
       } catch (IllegalAccessException ex) {
           ex.printStackTrace();
       }
   }

   private void doit() {
       Stuff s = new Stuff();
       info(s); s.prt();
       modify(s); s.prt();
       modify(s); s.prt();
   }

   public static void main(String[] args) {
       Reflect r = new Reflect();
       r.doit();
   }

}