class Xcopy{
	public static void main(String[] args){
		int orig = 20, new_int;
		Xcopy xcpy = new Xcopy();
		new_int = xcpy.go(20);
		System.out.println(orig + " " + new_int);
	}

	int go(int i){
		int ret = i *2; 
		return ret;
	}
}
