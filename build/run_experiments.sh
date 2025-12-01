#!/bin/bash

# Configurações (Ajustadas para sua imagem)
INSTANCES_DIR="../Instances/CVRP"  # Caminho relativo correto saindo de 'build'
EXEC="./hgs"                        # O nome do seu executável é 'hgs'
OUTPUT_LOG="resultados_experimento.csv"
NUM_RUNS=5
TIME_LIMIT=120 

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
targets["Instance_S_20_02.txt"]="1"
targets["Instance_S_20_03.txt"]="82"
targets["Instance_S_20_04.txt"]="13"
targets["Instance_S_20_05.txt"]="12452"

targets["Instance_S_22_01.txt"]="112"
targets["Instance_S_22_02.txt"]="140"
targets["Instance_S_22_03.txt"]="173"
targets["Instance_S_22_04.txt"]="160"
targets["Instance_S_22_05.txt"]="187"

targets["Instance_S_24_01.txt"]="11"
targets["Instance_S_24_02.txt"]="11"
targets["Instance_S_24_03.txt"]="10"
targets["Instance_S_24_04.txt"]="10"
targets["Instance_S_24_05.txt"]="89"

targets["Instance_S_50_01.txt"]="153"
targets["Instance_S_50_02.txt"]="146"
targets["Instance_S_50_03.txt"]="151"
targets["Instance_S_50_04.txt"]="154"
targets["Instance_S_50_05.txt"]="152"

targets["Instance_S_52_01.txt"]="145"
targets["Instance_S_52_02.txt"]="176"
targets["Instance_S_52_03.txt"]="153"
targets["Instance_S_52_04.txt"]="154"
targets["Instance_S_52_05.txt"]="145"

targets["Instance_S_54_01.txt"]="162"
targets["Instance_S_54_02.txt"]="183"
targets["Instance_S_54_03.txt"]="197"
targets["Instance_S_54_04.txt"]="894"
targets["Instance_S_54_05.txt"]="693"

targets["Instance_S_80_01.txt"]="341"
targets["Instance_S_80_02.txt"]="986"
targets["Instance_S_80_03.txt"]="317"
targets["Instance_S_80_04.txt"]="020"
targets["Instance_S_80_05.txt"]="892"

targets["Instance_S_82_01.txt"]="670"
targets["Instance_S_82_02.txt"]="854"
targets["Instance_S_82_03.txt"]="420"
targets["Instance_S_82_04.txt"]="944"
targets["Instance_S_82_05.txt"]="511"

targets["Instance_S_84_01.txt"]="993"
targets["Instance_S_84_02.txt"]="938"
targets["Instance_S_84_03.txt"]="734"
targets["Instance_S_84_04.txt"]="837"
targets["Instance_S_84_05.txt"]="824"

targets["Instance_S_100_01.txt"]="962"
targets["Instance_S_100_02.txt"]="446"
targets["Instance_S_100_03.txt"]="792"
targets["Instance_S_100_04.txt"]="509"
targets["Instance_S_100_05.txt"]="655"

targets["Instance_S_102_01.txt"]="938"
targets["Instance_S_102_02.txt"]="175"
targets["Instance_S_102_03.txt"]="137"
targets["Instance_S_102_04.txt"]="847"
targets["Instance_S_102_05.txt"]="434"

targets["Instance_S_104_01.txt"]="960"
targets["Instance_S_104_02.txt"]="517"
targets["Instance_S_104_03.txt"]="805"
targets["Instance_S_104_04.txt"]="876"
targets["Instance_S_104_05.txt"]="711"

targets["Instance_M_10_01.txt"]="6655"
targets["Instance_M_10_02.txt"]="8775"
targets["Instance_M_10_03.txt"]="8371"
targets["Instance_M_10_04.txt"]="6984"
targets["Instance_M_10_05.txt"]="5987"

targets["Instance_M_100_01.txt"]="159"
targets["Instance_M_100_02.txt"]="355"
targets["Instance_M_100_03.txt"]="801"
targets["Instance_M_100_04.txt"]="267"
targets["Instance_M_100_05.txt"]="969"

targets["Instance_M_102_01.txt"]="403"
targets["Instance_M_102_02.txt"]="450"
targets["Instance_M_102_03.txt"]="733"
targets["Instance_M_102_04.txt"]="073"
targets["Instance_M_102_05.txt"]="052"

targets["Instance_M_104_01.txt"]="694"
targets["Instance_M_104_02.txt"]="496"
targets["Instance_M_104_03.txt"]="171"
targets["Instance_M_104_04.txt"]="369"
targets["Instance_M_104_05.txt"]="378"

targets["Instance_M_12_01.txt"]="7707"
targets["Instance_M_12_02.txt"]="9135"
targets["Instance_M_12_03.txt"]="7868"
targets["Instance_M_12_04.txt"]="7614"
targets["Instance_M_12_05.txt"]="8872"

targets["Instance_M_14_01.txt"]="7410"
targets["Instance_M_14_02.txt"]="7076"
targets["Instance_M_14_03.txt"]="8779"
targets["Instance_M_14_04.txt"]="11123"
targets["Instance_M_14_05.txt"]="156"

targets["Instance_M_20_01.txt"]="151"
targets["Instance_M_20_02.txt"]="80"
targets["Instance_M_20_03.txt"]="89"
targets["Instance_M_20_04.txt"]="164"
targets["Instance_M_20_05.txt"]="122"

targets["Instance_M_22_01.txt"]="156"
targets["Instance_M_22_02.txt"]="185"
targets["Instance_M_22_03.txt"]="101"
targets["Instance_M_22_04.txt"]="176"
targets["Instance_M_22_05.txt"]="160"

targets["Instance_M_24_01.txt"]="126"
targets["Instance_M_24_02.txt"]="174"
targets["Instance_M_24_03.txt"]="110"
targets["Instance_M_24_04.txt"]="122"
targets["Instance_M_24_05.txt"]="80"

targets["Instance_M_50_01.txt"]="143"
targets["Instance_M_50_02.txt"]="181"
targets["Instance_M_50_03.txt"]="131"
targets["Instance_M_50_04.txt"]="101"
targets["Instance_M_50_05.txt"]="140"

targets["Instance_M_52_01.txt"]="151"
targets["Instance_M_52_02.txt"]="171"
targets["Instance_M_52_03.txt"]="145"
targets["Instance_M_52_04.txt"]="106"
targets["Instance_M_52_05.txt"]="173"

targets["Instance_M_54_01.txt"]="146"
targets["Instance_M_54_02.txt"]="140"
targets["Instance_M_54_03.txt"]="144"
targets["Instance_M_54_04.txt"]="182"
targets["Instance_M_54_05.txt"]="105"

targets["Instance_M_80_01.txt"]="221"
targets["Instance_M_80_02.txt"]="121"
targets["Instance_M_80_03.txt"]="177"
targets["Instance_M_80_04.txt"]="160"
targets["Instance_M_80_05.txt"]="276"

targets["Instance_M_82_01.txt"]="128"
targets["Instance_M_82_02.txt"]="134"
targets["Instance_M_82_03.txt"]="125"
targets["Instance_M_82_04.txt"]="150"
targets["Instance_M_82_05.txt"]="112"

targets["Instance_M_84_01.txt"]="173"
targets["Instance_M_84_02.txt"]="194"
targets["Instance_M_84_03.txt"]="147"
targets["Instance_M_84_04.txt"]="144"
targets["Instance_M_84_05.txt"]="1296"

targets["Instance_L_10_01.txt"]="6830"
targets["Instance_L_10_02.txt"]="8740"
targets["Instance_L_10_03.txt"]="8481"
targets["Instance_L_10_04.txt"]="6972"
targets["Instance_L_10_05.txt"]="6005"

targets["Instance_L_100_01.txt"]="666"
targets["Instance_L_100_02.txt"]="258"
targets["Instance_L_100_03.txt"]="512"
targets["Instance_L_100_04.txt"]="853"
targets["Instance_L_100_05.txt"]="879"

targets["Instance_L_102_01.txt"]="175"
targets["Instance_L_102_02.txt"]="417"
targets["Instance_L_102_03.txt"]="373"
targets["Instance_L_102_04.txt"]="888"
targets["Instance_L_102_05.txt"]="616"

targets["Instance_L_104_01.txt"]="203"
targets["Instance_L_104_02.txt"]="104"
targets["Instance_L_104_03.txt"]="193"
targets["Instance_L_104_04.txt"]="189"
targets["Instance_L_104_05.txt"]="211"

targets["Instance_L_12_01.txt"]="7953"
targets["Instance_L_12_02.txt"]="9259"
targets["Instance_L_12_03.txt"]="7926"
targets["Instance_L_12_04.txt"]="7543"
targets["Instance_L_12_05.txt"]="8990"

targets["Instance_L_14_01.txt"]="7654"
targets["Instance_L_14_02.txt"]="7085"
targets["Instance_L_14_03.txt"]="8963"
targets["Instance_L_14_04.txt"]="111"
targets["Instance_L_14_05.txt"]="92"

targets["Instance_L_20_01.txt"]="143"
targets["Instance_L_20_02.txt"]="887"
targets["Instance_L_20_03.txt"]="835"
targets["Instance_L_20_04.txt"]="1791"
targets["Instance_L_20_05.txt"]="1687"

targets["Instance_L_22_01.txt"]="1290"
targets["Instance_L_22_02.txt"]="1936"
targets["Instance_L_22_03.txt"]="1638"
targets["Instance_L_22_04.txt"]="1224"
targets["Instance_L_22_05.txt"]="1395"

targets["Instance_L_24_01.txt"]="1501"
targets["Instance_L_24_02.txt"]="1980"
targets["Instance_L_24_03.txt"]="929"
targets["Instance_L_24_04.txt"]="1558"
targets["Instance_L_24_05.txt"]="830"

targets["Instance_L_50_01.txt"]="1742"
targets["Instance_L_50_02.txt"]="1423"
targets["Instance_L_50_03.txt"]="1987"
targets["Instance_L_50_04.txt"]="1556"
targets["Instance_L_50_05.txt"]="1840"

targets["Instance_L_52_01.txt"]="1729"
targets["Instance_L_52_02.txt"]="1146"
targets["Instance_L_52_03.txt"]="1547"
targets["Instance_L_52_04.txt"]="1576"
targets["Instance_L_52_05.txt"]="1675"

targets["Instance_L_54_01.txt"]="1933"
targets["Instance_L_54_02.txt"]="1455"
targets["Instance_L_54_03.txt"]="1858"
targets["Instance_L_54_04.txt"]="1848"
targets["Instance_L_54_05.txt"]="1566"

targets["Instance_L_80_01.txt"]="2548"
targets["Instance_L_80_02.txt"]="1808"
targets["Instance_L_80_03.txt"]="1206"
targets["Instance_L_80_04.txt"]="1388"
targets["Instance_L_80_05.txt"]="2358"

targets["Instance_L_82_01.txt"]="1205"
targets["Instance_L_82_02.txt"]="1615"
targets["Instance_L_82_03.txt"]="1878"
targets["Instance_L_82_04.txt"]="1368"
targets["Instance_L_82_05.txt"]="1498"

targets["Instance_L_84_01.txt"]="1530"
targets["Instance_L_84_02.txt"]="1169"
targets["Instance_L_84_03.txt"]="1028"
targets["Instance_L_84_04.txt"]="1226"
targets["Instance_L_84_05.txt"]="1855"

 

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