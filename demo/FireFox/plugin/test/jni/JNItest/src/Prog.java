public class Prog {
	public Prog() {
		s = "test";
		System.out.println("call Prog()");
	}

	public void test() {
		System.out.println(s);
	}

	public String getS() {
		return s;
	}

	public void setS(String as) {
		s = as;
	}

	public static void main(String[] args) {
		System.out.println("Hello world " + args[0]);
	}

	String s;
}
