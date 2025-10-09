package TP04;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Scanner;

public class Game{

    //------ Atributos da classe Game--------

    public int id;
    public String name;
    public String releaseDate;
    public int estimatedOwners;
    public float price;
    public String[] supportedLanguages;
    public int metacriticScore;
    public float userScore;
    public int achievements; 
    public String[] publishers;
    public String[] developers;
    public String[] categories;
    public String[] genres;
    public String[] tags;


    //------ Construtor--------

    public Game(int id, String name, String releaseDate, int estimatedOwners, float price, String[] supportedLanguages, int metacriticScore, float userScore, int achievements, String[] publishers, String[] developers, String[] categories, String[] genres, String[] tags){
        this.id = id;
        this.name = name;
        this.releaseDate = releaseDate;
        this.estimatedOwners = estimatedOwners;
        this.price = price;
        this.supportedLanguages = supportedLanguages;
        this.metacriticScore = metacriticScore;
        this.userScore = userScore;
        this.achievements = achievements;
        this.publishers = publishers;
        this.developers = developers;
        this.categories = categories;
        this.genres = genres;
        this.tags = tags;
    }


    //----- Metodos Get--------

    public int getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public String getReleaseDate() {
        return releaseDate;
    }

    public int getEstimatedOwners() {
        return estimatedOwners;
    }

    public float getPrice() {
        return price;
    }

    public String[] getSupportedLanguages() {
        return supportedLanguages;
    }

    public int getMetacriticScore() {
        return metacriticScore;
    }

    public float getUserScore() {
        return userScore;
    }

    public int getAchievements() {
        return achievements;
    }

    public String[] getPublishers() {
        return publishers;
    }

    public String[] getDevelopers() {
        return developers;
    }

    public String[] getCategories() {
        return categories;
    }

    public String[] getGenres() {
        return genres;
    }

    public String[] getTags() {
        return tags;
    }


    //----- Tratamento de dados-------

    public static String formatArray(String[] array) {
        String resultado = "";

        if (array == null || array.length == 0) {
            resultado = "[]";
        } else {
            resultado = "[";
            for (int i = 0; i < array.length; i++) {
                resultado += array[i];
                if (i < array.length - 1) {
                    resultado += ", ";
                }
            }
            resultado += "]";
        }

        return resultado;
    }

    public static String removeEspacosBrancos(String s) {
        String resultado = "";
        if (s == null) {
            s = "";
        }

        int length = s.length();
        int inicio = 0;
        int fim = length - 1;
        boolean encontrouInicio = false;
        boolean encontrouFim = false;

        while (inicio < length && !encontrouInicio) {
            char c = s.charAt(inicio);
            if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
                encontrouInicio = true;
            } else {
                inicio++;
            }
        }

        while (fim >= inicio && !encontrouFim) {
            char c = s.charAt(fim);
            if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
                encontrouFim = true;
            } else {
                fim--;
            }
        }

        if (inicio <= fim) {
            for (int i = inicio; i <= fim; i++) {
                resultado += s.charAt(i);
            }
        }
        return resultado;
    }


    public static String[] splitPorAspas(String linhaCsv) {
        String[] campos = new String[14];
        String campoAtual = "";
        int indiceCampo = 0;
        boolean emAspas = false;
    
        int tamanhoLinha = linhaCsv.length();

        for (int i = 0; i < tamanhoLinha; i++) {
            char c = linhaCsv.charAt(i);

            // 1. Controle das aspas
            if (c == '"') {
                emAspas = !emAspas;
            }

            // 2. Condição de separação: Vírgula FORA das aspas
            if (c == ',' && !emAspas) {
                // Finaliza o campo
                if (indiceCampo < campos.length) {
                    campos[indiceCampo] = campoAtual;
                }
                campoAtual = "";
                indiceCampo++;
            } else {
                // Adiciona o caractere ao campo, mas ignora as aspas duplas de delimitação
                if (c != '"') {
                    campoAtual += c;
                }
            }
        }
        
        // 3. Adiciona o último campo (após o loop)
        if (indiceCampo < campos.length) {
            campos[indiceCampo] = campoAtual;
        }
        
        return campos;
    }

    public static Game leitura(String linhaCsv){
        String[] partesJogo = splitPorAspas(linhaCsv);

        int idTratado = Integer.parseInt(partesJogo[0]);

        String nomeTratado = partesJogo[1];

        String estimativaLimpa = trataEstimativa(partesJogo[3]);
        int estimativaTratada = estimativaLimpa.length() > 0 ? Integer.parseInt(estimativaLimpa) : 0;

        String dataTratada = trataData(partesJogo[2]);

        String precoLimpo = trataPreco(partesJogo[4]);
        float precoTratado = Float.parseFloat(precoLimpo);

        String[] linguagensTratadas = trataLinguagens(partesJogo[5]);

        String metacriticLimpo = trataNota(partesJogo[6], "-1");
        int metacriticTratado = Integer.parseInt(metacriticLimpo);

        String userScoreLimpo = trataNota(partesJogo[7], "-1.0");
        float userScoreTratado = Float.parseFloat(userScoreLimpo);

        String achievementesLimpo = trataAchievements(partesJogo[8]);
        int achievementesTratado = Integer.parseInt(achievementesLimpo);

        String[] publishersTratados = trataEmpresas(partesJogo[9]);

        String[] developersTratados = trataEmpresas(partesJogo[10]);

        String[] categoriesTratadas = trataListaColchetes(partesJogo[11]);

        String[] genresTratados = trataListaColchetes(partesJogo[12]);

        String[] tagsTratadas = trataListaColchetes(partesJogo[13]);

        Game novoJogo = new Game(idTratado, nomeTratado, dataTratada, estimativaTratada, precoTratado, linguagensTratadas, metacriticTratado, userScoreTratado, achievementesTratado, publishersTratados, developersTratados, categoriesTratadas, genresTratados, tagsTratadas);
        return novoJogo;
    }

    public static String[] trataListaColchetes(String listaTexto){
        String semCholchetes = "";
        int tamanho = listaTexto.length();

        if(tamanho > 2){
            for(int i = 0; i < tamanho; i++){
                semCholchetes += listaTexto.charAt(i);
            }
        }

        if(semCholchetes.length() == 0){
            String[] listaVazia = {};
            return listaVazia;
        }

        String[] listaItens = semCholchetes.split(",");
        return listaItens;
    }

    public static String[] trataEmpresas(String empresasTexto){
        String[] listaEmpresas = empresasTexto.split(",");
        return listaEmpresas;
    }

    public static String trataAchievements(String achievementsTexto){
        int tamanho = achievementsTexto.length();
        if(tamanho == 0){
            achievementsTexto = "0";
        }
        return achievementsTexto;
    }

    public static String trataPreco(String precoTexto){
        if(precoTexto.equals("Free to Play")){
            precoTexto = "0.0";
        }
        return precoTexto;
    }

    public static String[] trataLinguagens(String linguagensTexto){
        String semColchetesAspas = "";
        int tamanho = linguagensTexto.length();
        String[] linguagens;

        for(int i = 1; i < tamanho - 1; i++){
            char c = linguagensTexto.charAt(i);
            if (c != '\'') {
                semColchetesAspas += c;
            }
        }

        if (semColchetesAspas.length() == 0) {
            linguagens = new String[0];
        } else {
            linguagens = semColchetesAspas.split(",");
            for(int i = 0; i < linguagens.length; i++){
                linguagens[i] = removeEspacosBrancos(linguagens[i]);
            }
        }
        return linguagens;
    }

    public static String trataNota(String notaTexto, String valorVazio){
        if(notaTexto.length() == 0 || notaTexto.equals("tbd")){
            notaTexto = valorVazio;
        }
        return notaTexto;
    }

    public static String trataEstimativa(String estimativaUsuarios){
        String resultado = "";
        int tamanho = estimativaUsuarios.length();
        for(int i = 0; i < tamanho; i++){
            char caracter = estimativaUsuarios.charAt(i);
            if(caracter >= '0' && caracter <= '9'){
                resultado += caracter;
            }
        }
        return resultado;
    } 

    public static String trataData(String data) {
        if (data == null || data.isEmpty()) return "0000-00-00";

        String[] partes = data.split(" ");
        String mes = "01";
        String dia = "01";
        String ano = "0000";

        // Converter nome do mês para número
        switch (partes[0]) {
            case "Jan": mes = "01"; break;
            case "Feb": mes = "02"; break;
            case "Mar": mes = "03"; break;
            case "Apr": mes = "04"; break;
            case "May": mes = "05"; break;
            case "Jun": mes = "06"; break;
            case "Jul": mes = "07"; break;
            case "Aug": mes = "08"; break;
            case "Sep": mes = "09"; break;
            case "Oct": mes = "10"; break;
            case "Nov": mes = "11"; break;
            case "Dec": mes = "12"; break;
        }

        // Se há três partes (ex: "Dec 1, 2013")
        if (partes.length == 3) {
            dia = partes[1].replace(",", "");
            ano = partes[2];
        }
        // Se há duas partes (ex: "Dec 2013")
        else if (partes.length == 2) {
            ano = partes[1];
        }

        // Garantir formato YYYY-MM-DD
        if (dia.length() == 1) dia = "0" + dia;
        return dia + "/" + mes + "/" + ano;
    }


    //------- Funcoes de Impressao---------

    public void imprimeGame() {
        // Uso de System.out.printf para garantir a precisão dos floats (%.2f e %.1f)
        // e printar a linha completa, sem retornar String.
        
        System.out.printf("=> %d ## %s ## %s ## %d", 
            this.id, this.name, this.releaseDate, this.estimatedOwners);
        
        System.out.printf(" ## %.2f ## %s", 
            this.price, formatArray(this.supportedLanguages));
        
        System.out.printf(" ## %d ## %.1f ## %d", 
            this.metacriticScore, this.userScore, this.achievements);
            
        System.out.printf(" ## %s ## %s ## %s ## %s ## %s ##\n", 
            formatArray(this.publishers), 
            formatArray(this.developers), 
            formatArray(this.categories), 
            formatArray(this.genres), 
            formatArray(this.tags));
    }

    //------- Leitura CSV----------
    public static Game[] lerArquivoCSV(String caminhoArquivo) {
        Game[] jogos = new Game[0];
        BufferedReader leitor = null;
        String linha = "";
        int quantidade = 0;
        boolean primeiraLinha = true;

        try {
            leitor = new BufferedReader(new FileReader(caminhoArquivo));
            linha = leitor.readLine();

            while (linha != null) {
                if (!primeiraLinha) {
                    quantidade = quantidade + 1;
                } else {
                    primeiraLinha = false;
                }
                linha = leitor.readLine();
            }
            leitor.close();

            jogos = new Game[quantidade];
            leitor = new BufferedReader(new FileReader(caminhoArquivo));
            linha = leitor.readLine();
            primeiraLinha = true;

            int indice = 0;
            while (linha != null) {
                if (!primeiraLinha) {
                    Game jogo = Game.leitura(linha);
                    jogos[indice] = jogo;
                    indice = indice + 1;
                } else {
                    primeiraLinha = false;
                }
                linha = leitor.readLine();
            }

            leitor.close();
        } catch (IOException e) {
            System.out.println("Erro ao ler o arquivo: " + e.getMessage());
        }

        return jogos;
    }

    public static void buscarEImprimirJogo(Game[] jogos, String entrada) {
        int i = 0;
        int tamanho = jogos.length;
        String saida = "";

        while (i < tamanho) {
            String idTexto = "" + jogos[i].getId();
            if (idTexto.equals(entrada)) {
                jogos[i].imprimeGame();
                i = tamanho;
            } else {
                i = i + 1;
            }
        }

        if (saida.length() > 0) {
            System.out.println(saida);
        }
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        Game[] jogos = lerArquivoCSV("C:\\Users\\natan\\OneDrive\\Documentos\\ResolvendoTP\\games.csv");
        String entrada = sc.nextLine();

        while (!entrada.equals("FIM")) {
            buscarEImprimirJogo(jogos, entrada);
            entrada = sc.nextLine();
        }

        sc.close();
    }
}