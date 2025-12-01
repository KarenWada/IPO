#!/bin/bash

# Configurações (Ajustadas para sua imagem)
INSTANCES_DIR="../Instances/CVRP"  # Caminho relativo correto saindo de 'build'
EXEC="./hgs"                        # O nome do seu executável é 'hgs'
OUTPUT_LOG="resultados_experimento.csv"
NUM_RUNS=30
TIME_LIMIT=3600 

# Cria o cabeçalho do arquivo de resultados
echo "Instancia;Run;Tempo(s);CustoFinal;Target;AtingiuTarget?" > $OUTPUT_LOG

# ==============================================================================
# TABELA DE LIMITANTES SUPERIORES (LS) DO ARTIGO
# ==============================================================================
declare -A targets

# PREENCHA AQUI COM OS VALORES REAIS DO ARTIGO:
targets["Instance_S_10_01.txt"]="6705" 
targets["Instance_S_10_02.txt"]="8549" 
targets["Instance_S_10_03.txt"]="8233" 
targets["Instance_S_10_04.txt"]="6862" 
targets["Instance_S_10_05.txt"]="5829"

targets["Instance_S_12_01.txt"]="7785"
targets["Instance_S_12_02.txt"]="9045"
targets["Instance_S_12_03.txt"]="7831"
targets["Instance_S_12_04.txt"]="7301"
targets["Instance_S_12_05.txt"]="9089"

targets["Instance_S_14_01.txt"]="7529"
targets["Instance_S_14_02.txt"]="6885"
targets["Instance_S_14_03.txt"]="8928"
targets["Instance_S_14_04.txt"]="11159"
targets["Instance_S_14_05.txt"]="10153"

targets["Instance_S_20_01.txt"]="11294"
targets["Instance_S_20_02.txt"]="9383"
targets["Instance_S_20_03.txt"]="8602"
targets["Instance_S_20_04.txt"]="11193"
targets["Instance_S_20_05.txt"]="12452"

targets["Instance_S_22_01.txt"]="10812"
targets["Instance_S_22_02.txt"]="11440"
targets["Instance_S_22_03.txt"]="11173"
targets["Instance_S_22_04.txt"]="10660"
targets["Instance_S_22_05.txt"]="11787"

targets["Instance_S_24_01.txt"]="11795"
targets["Instance_S_24_02.txt"]="11156"
targets["Instance_S_24_03.txt"]="10374"
targets["Instance_S_24_04.txt"]="10857"
targets["Instance_S_24_05.txt"]="8922"

targets["Instance_S_50_01.txt"]="15963"
targets["Instance_S_50_02.txt"]="14606"
targets["Instance_S_50_03.txt"]="15781"
targets["Instance_S_50_04.txt"]="15614"
targets["Instance_S_50_05.txt"]="15072"

targets["Instance_S_52_01.txt"]="14925"
targets["Instance_S_52_02.txt"]="17246"
targets["Instance_S_52_03.txt"]="15973"
targets["Instance_S_52_04.txt"]="15044"
targets["Instance_S_52_05.txt"]="14951"

targets["Instance_S_54_01.txt"]="16276"
targets["Instance_S_54_02.txt"]="15583"
targets["Instance_S_54_03.txt"]="16097"
targets["Instance_S_54_04.txt"]="14894"
targets["Instance_S_54_05.txt"]="16693"

targets["Instance_S_80_01.txt"]="22341"
targets["Instance_S_80_02.txt"]="16986"
targets["Instance_S_80_03.txt"]="18317"
targets["Instance_S_80_04.txt"]="20020"
targets["Instance_S_80_05.txt"]="21892"

targets["Instance_S_82_01.txt"]="17670"
targets["Instance_S_82_02.txt"]="18854"
targets["Instance_S_82_03.txt"]="17420"
targets["Instance_S_82_04.txt"]="17944"
targets["Instance_S_82_05.txt"]="18511"

targets["Instance_S_84_01.txt"]="19993"
targets["Instance_S_84_02.txt"]="18938"
targets["Instance_S_84_03.txt"]="18734"
targets["Instance_S_84_04.txt"]="18837"
targets["Instance_S_84_05.txt"]="16824"

targets["Instance_S_100_01.txt"]="20962"
targets["Instance_S_100_02.txt"]="10446"
targets["Instance_S_100_03.txt"]="19792"
targets["Instance_S_100_04.txt"]="19509"
targets["Instance_S_100_05.txt"]="21655"

targets["Instance_S_102_01.txt"]="18938"
targets["Instance_S_102_02.txt"]="19175"
targets["Instance_S_102_03.txt"]="23137"
targets["Instance_S_102_04.txt"]="21847"
targets["Instance_S_102_05.txt"]="22434"

targets["Instance_S_104_01.txt"]="20960"
targets["Instance_S_104_02.txt"]="10517"
targets["Instance_S_104_03.txt"]="19805"
targets["Instance_S_104_04.txt"]="19876"
targets["Instance_S_104_05.txt"]="21711"

targets["Instance_M_10_01.txt"]="6655"
targets["Instance_M_10_02.txt"]="8775"
targets["Instance_M_10_03.txt"]="8371"
targets["Instance_M_10_04.txt"]="6984"
targets["Instance_M_10_05.txt"]="5987"

targets["Instance_M_100_01.txt"]="20159"
targets["Instance_M_100_02.txt"]="10355"
targets["Instance_M_100_03.txt"]="17801"
targets["Instance_M_100_04.txt"]="19267"
targets["Instance_M_100_05.txt"]="20969"

targets["Instance_M_102_01.txt"]="18403"
targets["Instance_M_102_02.txt"]="18450"
targets["Instance_M_102_03.txt"]="22733"
targets["Instance_M_102_04.txt"]="21073"
targets["Instance_M_102_05.txt"]="22052"

targets["Instance_M_104_01.txt"]="20694"
targets["Instance_M_104_02.txt"]="10496"
targets["Instance_M_104_03.txt"]="19171"
targets["Instance_M_104_04.txt"]="19369"
targets["Instance_M_104_05.txt"]="21378"

targets["Instance_M_12_01.txt"]="7707"
targets["Instance_M_12_02.txt"]="9135"
targets["Instance_M_12_03.txt"]="7868"
targets["Instance_M_12_04.txt"]="7614"
targets["Instance_M_12_05.txt"]="8872"

targets["Instance_M_14_01.txt"]="7410"
targets["Instance_M_14_02.txt"]="7076"
targets["Instance_M_14_03.txt"]="8779"
targets["Instance_M_14_04.txt"]="11123"
targets["Instance_M_14_05.txt"]="10156"

targets["Instance_M_20_01.txt"]="10851"
targets["Instance_M_20_02.txt"]="8900"
targets["Instance_M_20_03.txt"]="8649"
targets["Instance_M_20_04.txt"]="10964"
targets["Instance_M_20_05.txt"]="12022"

targets["Instance_M_22_01.txt"]="10456"
targets["Instance_M_22_02.txt"]="11185"
targets["Instance_M_22_03.txt"]="10801"
targets["Instance_M_22_04.txt"]="10376"
targets["Instance_M_22_05.txt"]="11660"

targets["Instance_M_24_01.txt"]="11626"
targets["Instance_M_24_02.txt"]="11174"
targets["Instance_M_24_03.txt"]="10310"
targets["Instance_M_24_04.txt"]="10822"
targets["Instance_M_24_05.txt"]="8820"

targets["Instance_M_50_01.txt"]="15843"
targets["Instance_M_50_02.txt"]="14581"
targets["Instance_M_50_03.txt"]="15731"
targets["Instance_M_50_04.txt"]="15601"
targets["Instance_M_50_05.txt"]="15040"

targets["Instance_M_52_01.txt"]="14851"
targets["Instance_M_52_02.txt"]="16971"
targets["Instance_M_52_03.txt"]="15745"
targets["Instance_M_52_04.txt"]="14806"
targets["Instance_M_52_05.txt"]="14773"

targets["Instance_M_54_01.txt"]="16146"
targets["Instance_M_54_02.txt"]="15540"
targets["Instance_M_54_03.txt"]="15944"
targets["Instance_M_54_04.txt"]="14882"
targets["Instance_M_54_05.txt"]="16705"

targets["Instance_M_80_01.txt"]="21621"
targets["Instance_M_80_02.txt"]="16621"
targets["Instance_M_80_03.txt"]="18277"
targets["Instance_M_80_04.txt"]="19560"
targets["Instance_M_80_05.txt"]="21276"

targets["Instance_M_82_01.txt"]="17328"
targets["Instance_M_82_02.txt"]="18234"
targets["Instance_M_82_03.txt"]="16625"
targets["Instance_M_82_04.txt"]="17550"
targets["Instance_M_82_05.txt"]="18012"

targets["Instance_M_84_01.txt"]="19773"
targets["Instance_M_84_02.txt"]="18294"
targets["Instance_M_84_03.txt"]="18447"
targets["Instance_M_84_04.txt"]="18344"
targets["Instance_M_84_05.txt"]="16296"

targets["Instance_L_10_01.txt"]="6830"
targets["Instance_L_10_02.txt"]="8740"
targets["Instance_L_10_03.txt"]="8481"
targets["Instance_L_10_04.txt"]="6972"
targets["Instance_L_10_05.txt"]="6005"

targets["Instance_L_100_01.txt"]="19666"
targets["Instance_L_100_02.txt"]="10258"
targets["Instance_L_100_03.txt"]="18512"
targets["Instance_L_100_04.txt"]="18853"
targets["Instance_L_100_05.txt"]="20879"

targets["Instance_L_102_01.txt"]="18175"
targets["Instance_L_102_02.txt"]="18417"
targets["Instance_L_102_03.txt"]="22373"
targets["Instance_L_102_04.txt"]="20888"
targets["Instance_L_102_05.txt"]="21616"

targets["Instance_L_104_01.txt"]="20373"
targets["Instance_L_104_02.txt"]="10491"
targets["Instance_L_104_03.txt"]="19318"
targets["Instance_L_104_04.txt"]="18962"
targets["Instance_L_104_05.txt"]="21139"

targets["Instance_L_12_01.txt"]="7953"
targets["Instance_L_12_02.txt"]="9259"
targets["Instance_L_12_03.txt"]="7926"
targets["Instance_L_12_04.txt"]="7543"
targets["Instance_L_12_05.txt"]="8990"

targets["Instance_L_14_01.txt"]="7654"
targets["Instance_L_14_02.txt"]="7085"
targets["Instance_L_14_03.txt"]="8963"
targets["Instance_L_14_04.txt"]="11199"
targets["Instance_L_14_05.txt"]="9792"

targets["Instance_L_20_01.txt"]="10443"
targets["Instance_L_20_02.txt"]="8687"
targets["Instance_L_20_03.txt"]="8435"
targets["Instance_L_20_04.txt"]="10791"
targets["Instance_L_20_05.txt"]="11687"

targets["Instance_L_22_01.txt"]="10290"
targets["Instance_L_22_02.txt"]="10936"
targets["Instance_L_22_03.txt"]="10638"
targets["Instance_L_22_04.txt"]="10224"
targets["Instance_L_22_05.txt"]="11395"

targets["Instance_L_24_01.txt"]="11501"
targets["Instance_L_24_02.txt"]="10980"
targets["Instance_L_24_03.txt"]="9929"
targets["Instance_L_24_04.txt"]="10558"
targets["Instance_L_24_05.txt"]="8730"

targets["Instance_L_50_01.txt"]="15742"
targets["Instance_L_50_02.txt"]="14423"
targets["Instance_L_50_03.txt"]="14987"
targets["Instance_L_50_04.txt"]="15556"
targets["Instance_L_50_05.txt"]="14840"

targets["Instance_L_52_01.txt"]="14729"
targets["Instance_L_52_02.txt"]="17146"
targets["Instance_L_52_03.txt"]="15547"
targets["Instance_L_52_04.txt"]="14576"
targets["Instance_L_52_05.txt"]="14675"

targets["Instance_L_54_01.txt"]="15933"
targets["Instance_L_54_02.txt"]="15455"
targets["Instance_L_54_03.txt"]="15858"
targets["Instance_L_54_04.txt"]="14848"
targets["Instance_L_54_05.txt"]="16566"

targets["Instance_L_80_01.txt"]="21548"
targets["Instance_L_80_02.txt"]="16808"
targets["Instance_L_80_03.txt"]="18206"
targets["Instance_L_80_04.txt"]="19388"
targets["Instance_L_80_05.txt"]="21358"

targets["Instance_L_82_01.txt"]="17205"
targets["Instance_L_82_02.txt"]="17615"
targets["Instance_L_82_03.txt"]="15878"
targets["Instance_L_82_04.txt"]="17368"
targets["Instance_L_82_05.txt"]="17498"

targets["Instance_L_84_01.txt"]="19530"
targets["Instance_L_84_02.txt"]="18169"
targets["Instance_L_84_03.txt"]="18028"
targets["Instance_L_84_04.txt"]="18226"
targets["Instance_L_84_05.txt"]="15855"

 

# ... adicione as outras

# ==============================================================================
# LOOP DE EXECUÇÃO
# ==============================================================================

for instance_path in $INSTANCES_DIR/*.txt; do
    
    instance_name=$(basename "$instance_path")
    
    # Busca o target na tabela. Se não tiver, usa 0.
    target_val=${targets[$instance_name]}
    if [ -z "$target_val" ]; then
        target_val=0
        echo "AVISO: Sem target para $instance_name. Rodando sem critério de parada."
    fi

    echo ">>> Iniciando testes para: $instance_name (Target: $target_val)"

    for ((i=1; i<=NUM_RUNS; i++)); do
        echo -n "   Run $i/$NUM_RUNS... "
        
        # Executa o algoritmo
        # Executa o algoritmo
        output=$($EXEC "$instance_path" "sol_temp.sol" -veh 1 -t $TIME_LIMIT -target "$target_val" -seed $i)
        
        # --- LÓGICA INTELIGENTE DE EXTRAÇÃO ---
        
        # 1. Verifica se atingiu o alvo (Procura a frase específica)
        if echo "$output" | grep -q "TARGET REACHED"; then
            hit_target="Sim"
            
            # Pega o custo da linha "----- TARGET REACHED: <custo> <="
            # O awk '{print $4}' pega a 4ª palavra da linha
            travel_time=$(echo "$output" | grep "TARGET REACHED" | awk '{print $4}')
            
            # Pega o tempo da linha "TIME SPENT: <tempo>"
            # O awk '{print $NF}' pega a última palavra da linha
            time_spent=$(echo "$output" | grep "TIME SPENT" | awk '{print $NF}')
            
        else
            hit_target="Nao"
            
            # Se não atingiu, pega a última linha de log normal "It ..."
            last_log=$(echo "$output" | grep "TravelTime:" | tail -n 1)
            
            # Extrai usando o Regex
            travel_time=$(echo "$last_log" | grep -oP 'TravelTime: \K[0-9.]+')
            time_spent=$(echo "$last_log" | grep -oP 'T\(s\) \K[0-9.]+')
        fi
        
        # Se por algum motivo o travel_time vier vazio (erro), coloca um placeholder
        if [ -z "$travel_time" ]; then travel_time="ERRO"; fi
        
        echo "Feito. Tempo: $time_spent s | Custo: $travel_time | Atingiu: $hit_target"
        
        # Salva no Excel (CSV)
        echo "$instance_name;$i;$time_spent;$travel_time;$target_val;$hit_target" >> $OUTPUT_LOG
        done
    echo "------------------------------------------------------------------"
done

echo "Resultados salvos em $OUTPUT_LOG"
#Lalala