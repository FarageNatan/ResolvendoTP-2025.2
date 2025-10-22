//package TP04;

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
        String resultado;
        String mes = "01";
        String dia = "01";
        String ano = "0000";

        if (data == null || data.length() == 0) {
            resultado = "0000-00-00";
        } else {
            String[] partes = data.split(" ");

            // Converter nome do mês para número
            if (partes.length > 0) {
                String mesNome = partes[0];
                switch (mesNome) {
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
            }

            // Se há três partes (ex: "Dec 1, 2013")
            if (partes.length == 3) {
                // remover a vírgula manualmente (não usamos replace)
                String diaComVirgula = partes[1];
                String diaLimpo = "";
                for (int i = 0; i < diaComVirgula.length(); i++) {
                    char c = diaComVirgula.charAt(i);
                    if (c != ',') {
                        diaLimpo += c;
                    }
                }
                dia = diaLimpo;
                ano = partes[2];
            }
            // Se há duas partes (ex: "Dec 2013")
            else if (partes.length == 2) {
                ano = partes[1];
            }

            // Garantir formato DD/MM/YYYY
            if (dia.length() == 1) {
                dia = "0" + dia;
            }

            resultado = dia + "/" + mes + "/" + ano;
        }

        return resultado;
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

    // Busca e retorna o Game correspondente ao id informado (ou null se não encontrado).
    // OBS: apenas um return por método.
    public static Game getGameById(Game[] jogos, String entradaId) {
        Game encontrado = null;
        int i = 0;
        int tamanho = jogos.length;
        while (i < tamanho) {
            String idTexto = "" + jogos[i].getId();
            if (idTexto.equals(entradaId)) {
                encontrado = jogos[i];
                i = tamanho;
            } else {
                i = i + 1;
            }
        }
        return encontrado;
    }

    public static Game[] inserirFim(Game[] arrayOriginal, Game elemento) {
        Game[] novo = new Game[arrayOriginal.length + 1];
        for (int i = 0; i < arrayOriginal.length; i++) {
            novo[i] = arrayOriginal[i];
        }
        novo[arrayOriginal.length] = elemento;
        return novo;
    }

    public static Game[] ordenaSelecaoPorNome(Game[] arrayOriginal){
        int tamanho = arrayOriginal.length;
        for (int i = 0; i < (tamanho - 1); i++) {
            int menor = i;
            for (int j = (i + 1); j < tamanho; j++){
                if (arrayOriginal[j].getName().compareTo(arrayOriginal[menor].getName()) < 0){
                    menor = j;
                }
            }
            swapGames(arrayOriginal, menor, i);
        }
        return arrayOriginal;
    }

    public static void swapGames(Game[] array, int i, int j) {
        Game temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }

    public static boolean pesquisaBinaria(Game[] array, String chave) {
        boolean encontrou = false;
        int esquerda = 0;
        int direita = array.length - 1;

        while (esquerda <= direita && !encontrou) {
            int meio = (esquerda + direita) / 2;
            int cmp = chave.compareTo(array[meio].getName());
            if (cmp == 0) {
                encontrou = true;
            } else if (cmp < 0) {
                direita = meio - 1;
            } else {
                esquerda = meio + 1;
            }
        }

        return encontrou;
    }

    public static void mergeSortPorPreco(Game[] array) {
        if (array == null || array.length < 2) {
            return;
        }
        Game[] aux = new Game[array.length];
        mergeSortRec(array, aux, 0, array.length - 1);
    }

    private static void mergeSortRec(Game[] array, Game[] aux, int left, int right) {
        if (left >= right) {
            return;
        }
        int mid = (left + right) / 2;
        mergeSortRec(array, aux, left, mid);
        mergeSortRec(array, aux, mid + 1, right);
        merge(array, aux, left, mid, right);
    }

    private static void merge(Game[] array, Game[] aux, int left, int mid, int right) {
        int i = left;
        int j = mid + 1;
        int k = left;

        for (int p = left; p <= right; p++) {
            aux[p] = array[p];
        }

        while (i <= mid && j <= right) {
            // comparar por price asc; se empatar, por id asc
            boolean escolherLeft;
            if (aux[i].getPrice() < aux[j].getPrice()) {
                escolherLeft = true;
            } else if (aux[i].getPrice() > aux[j].getPrice()) {
                escolherLeft = false;
            } else {
                // empate no price -> desempata por id
                if (aux[i].getId() <= aux[j].getId()) {
                    escolherLeft = true;
                } else {
                    escolherLeft = false;
                }
            }

            if (escolherLeft) {
                array[k] = aux[i];
                i = i + 1;
            } else {
                array[k] = aux[j];
                j = j + 1;
            }
            k = k + 1;
        }

        // copiar o restante da metade esquerda (se houver)
        while (i <= mid) {
            array[k] = aux[i];
            i = i + 1;
            k = k + 1;
        }

        // restante da metade direita já está no lugar, porque copiamos do aux
        while (j <= right) {
            array[k] = aux[j];
            j = j + 1;
            k = k + 1;
        }
    }

    // ------- HEAPSORT por estimatedOwners (primário) e id (secundário) -------

    // compara a e b: retorna -1 se a<b, 1 se a>b, 0 se iguais (apenas 1 return)
    private static int compararPorEstimatedOwners(Game jogo1, Game jogo2) {
        int resultado;
        if (jogo1.getEstimatedOwners() < jogo2.getEstimatedOwners()) {
            resultado = -1;
        } else if (jogo1.getEstimatedOwners() > jogo2.getEstimatedOwners()) {
            resultado = 1;
        } else {
            // empate em estimatedOwners -> desempata por id
            if (jogo1.getId() < jogo2.getId()) {
                resultado = -1;
            } else if (jogo1.getId() > jogo2.getId()) {
                resultado = 1;
            } else {
                resultado = 0;
            }
        }
        return resultado;
    }

    // heapify (sift-down) para max-heap usando compararPorEstimatedOwners
    private static void heapify(Game[] array, int n, int i) {
        int maior = i;
        boolean teste = true;
        while (teste) {
            int l = 2 * i + 1;
            int r = 2 * i + 2;
            maior = i;

            if (l < n) {
                if (compararPorEstimatedOwners(array[l], array[maior]) > 0) {
                    maior = l;
                }
            }

            if (r < n) {
                if (compararPorEstimatedOwners(array[r], array[maior]) > 0) {
                    maior = r;
                }
            }

            if (maior != i) {
                swapGames(array, i, maior);
                i = maior;
            } else {
                teste = false;
            }
        }
    }

    // heapsort: constrói max-heap e extrai para ordenar de forma ascendente ao final
    public static void heapSortPorEstimatedOwners(Game[] array) {
        if (array == null || array.length < 2) {
            return;
        }

        int tamanho = array.length;

        // construir max-heap
        for (int i = (tamanho / 2) - 1; i >= 0; i--) {
            heapify(array, tamanho, i);
        }

        // extrair elementos do heap
        for (int i = tamanho - 1; i >= 1; i--) {
            // mover raiz (maior) para o fim
            swapGames(array, 0, i);
            // reduzir heap e heapify na raiz
            heapify(array, i, 0);
        }
    }


    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        Game[] jogos = lerArquivoCSV("/tmp/games.csv");
        //Game[] jogos = lerArquivoCSV("C:\\Users\\natan\\OneDrive\\Documentos\\ResolvendoTP\\games.csv");

        // PRIMEIRA PARTE: entradas de IDs (igual TP-04) -> inserir os jogos no final do vetor 'selecionados'
        Game[] selecionados = new Game[0];
        String entrada = sc.nextLine();
        while (!entrada.equals("FIM")) {
            Game encontrado = getGameById(jogos, entrada);
            if (encontrado != null) {
                selecionados = inserirFim(selecionados, encontrado);
            }
            entrada = sc.nextLine();
        }

        // Ordenar os selecionados pelo name antes das pesquisas
        selecionados = ordenaSelecaoPorNome(selecionados);

        // SEGUNDA PARTE: linhas com nomes a pesquisar
        String consulta = sc.nextLine();
        while (!consulta.equals("FIM")) {
            boolean achou = pesquisaBinaria(selecionados, consulta);
            if (achou) {
                System.out.println(" SIM");
            } else {
                System.out.println(" NAO");
            }
            consulta = sc.nextLine();
        }

        sc.close();
    }
}