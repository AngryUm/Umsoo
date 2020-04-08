import java.util.Scanner;

public class Multiple {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Scanner input = new Scanner ( System.in );
		int number;
		System.out.print("Please enter a non-negative (>= 0) integer number : ");
		number = input.nextInt();
		
		if(((number%2) == 0) && ((number%13) == 0)) {
			System.out.println(number+":MULTIPLE:EVEN");
		}
		else if((number%2 != 0) && ((number%13) == 0)) {
			System.out.println(number+":MULTIPLE:ODD");
		}
		else if((number%2 == 0) && ((number%13) != 0)) {
			System.out.println(number+":NOT:EVEN");
		}
		else if((number%2 != 0) && ((number%13) != 0)) {
			System.out.println(number+":NOT:ODD");
		}
	}

}
