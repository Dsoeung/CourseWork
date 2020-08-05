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

	}
	
	private void modify(Object obj) {
		try {
      // TBD: Enter code here to change the object
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
