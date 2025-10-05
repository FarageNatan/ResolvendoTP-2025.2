package TP04;

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
                    campoAtual += c; // Concatenação de String permitida
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
            for(int i = 1; i < tamanho - 1; i++){
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
        String semColchetes = "";
        int tamanho = linguagensTexto.length();
        for(int i = 1; i < tamanho - 1; i++){
            semColchetes += linguagensTexto.charAt(i);
        }
        String[] linguagens = semColchetes.split(",");
        return linguagens;
    }

    public static String trataNota(String notaTexto, String valorVazio){
        if(notaTexto.length() == 0 || notaTexto.equals("tbd")){
            notaTexto = valorVazio;
        }
        return notaTexto;
    }

    public static String trataEstimativa(String estimativaUsuarios){ //remove caracteres que nao sao digitos
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

    public static String trataData(String dataOriginal){
        String[] partesData = dataOriginal.split(" ");

        String mesAbreviado = partesData[0];
        String diaVirgula = partesData[1];
        String ano = partesData[2];

        String dia = "";
        int tamanhoDia = diaVirgula.length();
        for(int i = 0; i < tamanhoDia; i++){
            char caracter = diaVirgula.charAt(i);
            if(caracter != ',') dia += caracter;
        }

        String mesNumero;
        if (mesAbreviado.equals("Jan")) {
            mesNumero = "01";
        } else if (mesAbreviado.equals("Feb")) {
            mesNumero = "02";
        } else if (mesAbreviado.equals("Mar")) {
            mesNumero = "03";
        } else if (mesAbreviado.equals("Apr")) {
            mesNumero = "04";
        } else if (mesAbreviado.equals("May")) {
            mesNumero = "05";
        } else if (mesAbreviado.equals("Jun")) {
            mesNumero = "06";
        } else if (mesAbreviado.equals("Jul")) {
            mesNumero = "07";
        } else if (mesAbreviado.equals("Aug")) {
            mesNumero = "08";
        } else if (mesAbreviado.equals("Sep")) {
            mesNumero = "09";
        } else if (mesAbreviado.equals("Oct")) {
            mesNumero = "10";
        } else if (mesAbreviado.equals("Nov")) {
            mesNumero = "11";
        } else if (mesAbreviado.equals("Dec")) {
            mesNumero = "12";
        } else {
            // Se o mês for desconhecido ou estiver incompleto (Regra: usa "01" se faltar)
            mesNumero = "01"; 
        }

        if(dia.length() == 1){
            dia = "0" + dia;
        }
        
        String dataFormatada = dia + "/" + mesNumero + "/" + ano;

        return dataFormatada;
    }

    //------- Funcoes de Impressao---------

    public static String formatarArray(String[] array){
        String resultado = "";
        if(array == null || array.length == 0 || (array.length == 1 && array[0] != null && array[0].trim().isEmpty())){
            resultado = "[]";
        }
        
        resultado = "[";
        int tamanho = array.length;
        for(int i = 0; i < tamanho; i++){
            resultado += array
        }
        
    }



}