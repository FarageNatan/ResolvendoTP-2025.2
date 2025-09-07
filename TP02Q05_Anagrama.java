import java.util.Scanner;

public class TP02Q05_Anagrama {
    
	/*
	 * FUNÇÃO VERIFICA FIM
	 */
	public static boolean verificaFim(String str, String chave) {
		boolean fim = false;

		if (str.length() == chave.length()) {
			fim = true;

			for (int i=0; i < chave.length(); i++) {
				if (str.charAt(i) != chave.charAt(i)) {
					fim = false;
					i = chave.length();
				}
			}
		}

		return fim;
	}

	/*
	 * FUNÇÃO VERIFICA FIM BASE
	 */
	public static boolean verificaFim(String str) {
		return verificaFim(str, "FIM");
	}

	/*
	 * FUNÇÃO É ANAGRAMA
	 */
	public static boolean ehAnagrama(String str1, String str2) {
		boolean eh = true;
		
		if (str1.length() != str2.length()) {
			eh = false;
		}
		else {
			int cont = 0;

			for (int i=0; i < str1.length(); i++) { // laço da primeira string
				for (int j=0; j < str2.length(); j++) { // laço da segunda string
					
					// if caso as duas letras sejam iguais contabilizar o contador
					if (str1.charAt(i) == str2.charAt(j)) {
						cont++;
						j = str2.length();
					}

					// else if para caso a letra da primeira string seja maíuscula
					else if (str1.charAt(i) >= 65 && str1.charAt(i) <= 90) {
						char str1Min = (char) (str1.charAt(i) + 32); // converte a letra maíuscula para minúscula de acordo com a tabela ASCII
						
						if (str1Min == str2.charAt(j)) { 
							cont++;
							j = str2.length();
						}
					}

					// else if para caso a letra da segunda string seja maíuscula
					else if (str2.charAt(j) >= 65 && str2.charAt(j) <= 90) {
						char str2Min = (char) (str2.charAt(j) + 32); // converte a letra maísculo para minúscula de acordo com a tabela ASCII

						if (str1.charAt(i) == str2Min) {
							cont++;
							j = str2.length();
						}
					}
				}
			}

			// verificação se o contador é igual ao tamanho da string
			if (cont != str1.length()) {
				eh = false;
			}
		}

		return eh;
	}

	/*
	 * FUNÇÃO MAIN
	 */
	public static void main(String[] args) {
		Scanner sc = new Scanner(System.in);
		String str1 = sc.next();
		String str2 = "";

		while (!verificaFim(str1)) {
			sc.next();
			str2 = sc.next();

 			System.out.println((ehAnagrama(str1, str2)) ? "SIM" : "N\u00C3O");
			str1 = sc.next();
		}

		sc.close();
	}
}
