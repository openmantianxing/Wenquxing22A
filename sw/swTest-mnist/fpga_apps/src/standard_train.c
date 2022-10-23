#include "../include/snn.h"
#include "../include/fpga_data/mn_d_0.h"
#include "../include/fpga_data/mn_d_1.h"
#include "../include/fpga_data/mn_d_2.h"
#include "../include/fpga_data/mn_d_3.h"
#include "../include/fpga_data/mn_d_4.h"
#include "../include/fpga_data/mn_d_5.h"
#include "../include/fpga_data/mn_d_6.h"
#include "../include/fpga_data/mn_d_7.h"
#include "../include/fpga_data/mn_d_8.h"
#include "../include/fpga_data/mn_d_9.h"


#define WEXP            256
#define ITE_PER_SAMPLE  100
#define N_SYN64         12
#define N_NEU           10
#define VLEAK           15
#define VTH             30
#define VINIT           0 << 1
#define SNN_EN          1

int main(void){
    snn64_t syn[10][12] = {
    {4697316039218070286ULL, 8792339837987738909ULL, 1227727241677820147ULL, 6397601176760867387ULL, 16175816958309440576ULL, 16036384737343242680ULL, 5730513867442174874ULL, 2513177344660065431ULL, 18342348402792034077ULL, 13917890387437693791ULL, 18119846723355555273ULL, 10279055070812741487ULL}, 
    {13092104170705209922ULL, 10571591920005050464ULL, 13388171216999418836ULL, 14575225269866684190ULL, 472345949065531517ULL, 8663088712882534681ULL, 17739257271272123696ULL, 16849226962936475874ULL, 8807811593336957424ULL, 17979093587951515953ULL, 8765638232835301884ULL, 17303862742246186451ULL}, 
    {15419453725688569621ULL, 4052878379170151030ULL, 17676245314068974757ULL, 8715847248984954795ULL, 10724155584449155563ULL, 4747508945443044001ULL, 16462759583990426142ULL, 10935622810429802897ULL, 8487216675612463082ULL, 16427219633411501165ULL, 4020623374865432321ULL, 6421732140897041142ULL}, 
    {10096867050163091902ULL, 10698927001699565247ULL, 15197722791093957141ULL, 7898904960789938357ULL, 7835859996470767625ULL, 10223634911205563699ULL, 13490591716331870474ULL, 11231143979555381993ULL, 10217427540254641175ULL, 9797282438606930221ULL, 12711174591986201140ULL, 8087528083777453923ULL}, 
    {831647257744779798ULL, 16602496713260367960ULL, 453549265036072234ULL, 17627450761650990769ULL, 4274502202284745608ULL, 1538119052497390154ULL, 3514618273004121173ULL, 14235120802862261160ULL, 914287480736977979ULL, 14127416517799385076ULL, 15237736206047528451ULL, 1532385071549378834ULL}, 
    {12640486834076156519ULL, 9079222920674287277ULL, 6583343615352202111ULL, 10785321426331600308ULL, 9494382368904576135ULL, 13195308855861255146ULL, 17423398336274047455ULL, 10567852460105023837ULL, 14090813924330957728ULL, 12628830878621991417ULL, 4936931068849317907ULL, 8770459477130754564ULL}, 
    {16588109431117099444ULL, 3037819808733741133ULL, 14326601524167006347ULL, 15959121592793476834ULL, 15810099508253212411ULL, 1948936891152212636ULL, 16352755451721056507ULL, 14834264651583254643ULL, 16031726680331445212ULL, 17871547216656023137ULL, 5964265084253668506ULL, 17446801535877336589ULL}, 
    {17490272465308054039ULL, 445895010212623367ULL, 15004317207835235110ULL, 11988086987357003863ULL, 8320992105873708308ULL, 10453782335473349376ULL, 13232341890803589245ULL, 10077524689680428210ULL, 10066357586247384442ULL, 12909625070948576674ULL, 16867967531189992793ULL, 16900628554073068013ULL}, 
    {14902995763730999283ULL, 4241607118532134482ULL, 15278197467973210799ULL, 12504565239153362974ULL, 18235472895710040753ULL, 15793606500272835198ULL, 9373718883100153015ULL, 14754245209833945040ULL, 8218749255789032560ULL, 17648787360825831436ULL, 16934223459273659394ULL, 8630676151886879064ULL}, 
    {10695027613006924853ULL, 3945466246042404086ULL, 15910343435363565897ULL, 14517758829510354760ULL, 17590134313086550422ULL, 8270863297202113675ULL, 14225349235125363751ULL, 6210695584239059501ULL, 18228859201110506920ULL, 15049008281958538453ULL, 16732462523834774938ULL, 7622758816271009715ULL}
    };
    snn64_t vneuron[10] = {
        VINIT, VINIT, VINIT, VINIT, VINIT, VINIT, VINIT, VINIT, VINIT, VINIT
    };
    sinit(VINIT, SNN_EN);
    vleak(VLEAK);


    snn64_t vin;
    snn64_t valid_spikes;
    snn64_t output;
    snn64_t vneuron_update;
    snn64_t syn_update;
    snn64_t wsum;
    snn64_t dw;
    snn64_t possible;
    int inf_num;
    // digit training 0
    inf_num = 0x1;
    for(int i = 0; i < N_0; i++){
        //printf("\n%d\n", i);
        for(int j = 0; j < ITE_PER_SAMPLE; j ++){
            //printf("%d\n", j);
            vin = 0;
            valid_spikes = 0;
            output = 0;
            syn_update = 0;
            dw = 0;
            for(int k =0; k < N_SYN64; k++){
                
                valid_spikes = ands(syn[0][k], spikes_0[i][j][k]);
                
                //printf("valid_spikes: 0x%08x \n", valid_spikes);

                vin = rpop(valid_spikes);

                //printf("vin: 0x%08x \n", vin);

                vneuron_update = sge(nadd(vneuron[0], vin), VTH);

                //printf("vneuron_update: 0x%08x \n", vneuron_update);

                vneuron[0] = vneuron_update;

                //printf("vneuron: 0x%08x \n", vneuron[0]);

                output = sls(output);
                
                //printf("%d\n", k);

                //printf("output: 0x%08x \n", output);
                inf(inf_num);
                syn_update = sup(syn[0][k],spikes_0[i][j][k]);

                //printf("syn_update: 0x%08x \n", syn_update);

                wsum = rpop(syn_update);

                //printf("wsum: 0x%08x \n", wsum);

                dw = wsum > WEXP ? wsum - WEXP : 0;
                possible = dw * 1024 / wsum;
                syn_update = ltd(possible, syn_update);

                //printf("syn_update: 0x%08x \n", syn_update);

                syn[0][k] = syn_update;

                //printf("syn 0x%08x \n", syn[0][k]);

            }
        }
    }

    // digit training 1
    inf_num <<= 1;
    for(int i = 0; i < N_0; i++){
        //printf("\n%d\n", i);
        for(int j = 0; j < ITE_PER_SAMPLE; j ++){
            //printf("%d\n", j);
            vin = 0;
            valid_spikes = 0;
            output = 0;
            syn_update = 0;
            dw = 0;
            for(int k =0; k < N_SYN64; k++){
                
                valid_spikes = ands(syn[1][k], spikes_1[i][j][k]);
                
                //printf("valid_spikes: 0x%08x \n", valid_spikes);

                vin = rpop(valid_spikes);

                //printf("vin: 0x%08x \n", vin);

                vneuron_update = sge(nadd(vneuron[1], vin), VTH);

                //printf("vneuron_update: 0x%08x \n", vneuron_update);

                vneuron[1] = vneuron_update;

                //printf("vneuron: 0x%08x \n", vneuron[0]);

                output = sls(output);
                
                //printf("%d\n", k);

                //printf("output: 0x%08x \n", output);
                inf(inf_num);
                syn_update = sup(syn[1][k],spikes_1[i][j][k]);

                //printf("syn_update: 0x%08x \n", syn_update);

                wsum = rpop(syn_update);

                //printf("wsum: 0x%08x \n", wsum);

                dw = wsum > WEXP ? wsum - WEXP : 0;
                possible = dw * 1024 / wsum;
                syn_update = ltd(possible, syn_update);

                //printf("syn_update: 0x%08x \n", syn_update);

                syn[1][k] = syn_update;

                //printf("syn 0x%08x \n", syn[0][k]);

            }
        }
    }

    // digit training 2
    inf_num <<= 1;
    for(int i = 0; i < N_2; i++){
        //printf("\n%d\n", i);
        for(int j = 0; j < ITE_PER_SAMPLE; j ++){
            //printf("%d\n", j);
            vin = 0;
            valid_spikes = 0;
            output = 0;
            syn_update = 0;
            dw = 0;
            for(int k =0; k < N_SYN64; k++){
                
                valid_spikes = ands(syn[2][k], spikes_2[i][j][k]);
                
                //printf("valid_spikes: 0x%08x \n", valid_spikes);

                vin = rpop(valid_spikes);

                //printf("vin: 0x%08x \n", vin);

                vneuron_update = sge(nadd(vneuron[2], vin), VTH);

                //printf("vneuron_update: 0x%08x \n", vneuron_update);

                vneuron[2] = vneuron_update;

                //printf("vneuron: 0x%08x \n", vneuron[0]);

                output = sls(output);
                
                //printf("%d\n", k);

                //printf("output: 0x%08x \n", output);
                inf(inf_num);
                syn_update = sup(syn[2][k],spikes_2[i][j][k]);

                //printf("syn_update: 0x%08x \n", syn_update);

                wsum = rpop(syn_update);

                //printf("wsum: 0x%08x \n", wsum);

                dw = wsum > WEXP ? wsum - WEXP : 0;
                possible = dw * 1024 / wsum;
                syn_update = ltd(possible, syn_update);

                //printf("syn_update: 0x%08x \n", syn_update);

                syn[2][k] = syn_update;

                //printf("syn 0x%08x \n", syn[0][k]);

            }
        }
    }

    // digit training 3
    inf_num <<= 1;
    for(int i = 0; i < N_3; i++){
        //printf("\n%d\n", i);
        for(int j = 0; j < ITE_PER_SAMPLE; j ++){
            //printf("%d\n", j);
            vin = 0;
            valid_spikes = 0;
            output = 0;
            syn_update = 0;
            dw = 0;
            for(int k =0; k < N_SYN64; k++){
                
                valid_spikes = ands(syn[3][k], spikes_3[i][j][k]);
                
                //printf("valid_spikes: 0x%08x \n", valid_spikes);

                vin = rpop(valid_spikes);

                //printf("vin: 0x%08x \n", vin);

                vneuron_update = sge(nadd(vneuron[3], vin), VTH);

                //printf("vneuron_update: 0x%08x \n", vneuron_update);

                vneuron[3] = vneuron_update;

                //printf("vneuron: 0x%08x \n", vneuron[0]);

                output = sls(output);
                
                //printf("%d\n", k);

                //printf("output: 0x%08x \n", output);
                inf(inf_num);
                syn_update = sup(syn[3][k],spikes_3[i][j][k]);

                //printf("syn_update: 0x%08x \n", syn_update);

                wsum = rpop(syn_update);

                //printf("wsum: 0x%08x \n", wsum);

                dw = wsum > WEXP ? wsum - WEXP : 0;
                possible = dw * 1024 / wsum;
                syn_update = ltd(possible, syn_update);

                //printf("syn_update: 0x%08x \n", syn_update);

                syn[3][k] = syn_update;

                //printf("syn 0x%08x \n", syn[0][k]);

            }
        }
    }

    // digit training 4
    inf_num <<= 1;
    for(int i = 0; i < N_4; i++){
        //printf("\n%d\n", i);
        for(int j = 0; j < ITE_PER_SAMPLE; j ++){
            //printf("%d\n", j);
            vin = 0;
            valid_spikes = 0;
            output = 0;
            syn_update = 0;
            dw = 0;
            for(int k =0; k < N_SYN64; k++){
                
                valid_spikes = ands(syn[4][k], spikes_4[i][j][k]);
                
                //printf("valid_spikes: 0x%08x \n", valid_spikes);

                vin = rpop(valid_spikes);

                //printf("vin: 0x%08x \n", vin);

                vneuron_update = sge(nadd(vneuron[4], vin), VTH);

                //printf("vneuron_update: 0x%08x \n", vneuron_update);

                vneuron[4] = vneuron_update;

                //printf("vneuron: 0x%08x \n", vneuron[0]);

                output = sls(output);
                
                //printf("%d\n", k);

                //printf("output: 0x%08x \n", output);
                inf(inf_num);
                syn_update = sup(syn[4][k],spikes_4[i][j][k]);

                //printf("syn_update: 0x%08x \n", syn_update);

                wsum = rpop(syn_update);

                //printf("wsum: 0x%08x \n", wsum);

                dw = wsum > WEXP ? wsum - WEXP : 0;
                possible = dw * 1024 / wsum;
                syn_update = ltd(possible, syn_update);

                //printf("syn_update: 0x%08x \n", syn_update);

                syn[4][k] = syn_update;

                //printf("syn 0x%08x \n", syn[0][k]);

            }
        }
    }

    // digit training 5
    inf_num <<= 1;
    for(int i = 0; i < N_5; i++){
        //printf("\n%d\n", i);
        for(int j = 0; j < ITE_PER_SAMPLE; j ++){
            //printf("%d\n", j);
            vin = 0;
            valid_spikes = 0;
            output = 0;
            syn_update = 0;
            dw = 0;
            for(int k =0; k < N_SYN64; k++){
                
                valid_spikes = ands(syn[5][k], spikes_5[i][j][k]);
                
                //printf("valid_spikes: 0x%08x \n", valid_spikes);

                vin = rpop(valid_spikes);

                //printf("vin: 0x%08x \n", vin);

                vneuron_update = sge(nadd(vneuron[5], vin), VTH);

                //printf("vneuron_update: 0x%08x \n", vneuron_update);

                vneuron[5] = vneuron_update;

                //printf("vneuron: 0x%08x \n", vneuron[0]);

                output = sls(output);
                
                //printf("%d\n", k);

                //printf("output: 0x%08x \n", output);
                inf(inf_num);
                syn_update = sup(syn[5][k],spikes_5[i][j][k]);

                //printf("syn_update: 0x%08x \n", syn_update);

                wsum = rpop(syn_update);

                //printf("wsum: 0x%08x \n", wsum);

                dw = wsum > WEXP ? wsum - WEXP : 0;
                possible = dw * 1024 / wsum;
                syn_update = ltd(possible, syn_update);

                //printf("syn_update: 0x%08x \n", syn_update);

                syn[5][k] = syn_update;

                //printf("syn 0x%08x \n", syn[0][k]);

            }
        }
    }

    // digit training 6
    inf_num <<= 1;
    for(int i = 0; i < N_6; i++){
        //printf("\n%d\n", i);
        for(int j = 0; j < ITE_PER_SAMPLE; j ++){
            //printf("%d\n", j);
            vin = 0;
            valid_spikes = 0;
            output = 0;
            syn_update = 0;
            dw = 0;
            for(int k =0; k < N_SYN64; k++){
                
                valid_spikes = ands(syn[6][k], spikes_6[i][j][k]);
                
                //printf("valid_spikes: 0x%08x \n", valid_spikes);

                vin = rpop(valid_spikes);

                //printf("vin: 0x%08x \n", vin);

                vneuron_update = sge(nadd(vneuron[6], vin), VTH);

                //printf("vneuron_update: 0x%08x \n", vneuron_update);

                vneuron[6] = vneuron_update;

                //printf("vneuron: 0x%08x \n", vneuron[0]);

                output = sls(output);
                
                //printf("%d\n", k);

                //printf("output: 0x%08x \n", output);
                inf(inf_num);
                syn_update = sup(syn[6][k],spikes_6[i][j][k]);

                //printf("syn_update: 0x%08x \n", syn_update);

                wsum = rpop(syn_update);

                //printf("wsum: 0x%08x \n", wsum);

                dw = wsum > WEXP ? wsum - WEXP : 0;
                possible = dw * 1024 / wsum;
                syn_update = ltd(possible, syn_update);

                //printf("syn_update: 0x%08x \n", syn_update);

                syn[6][k] = syn_update;

                //printf("syn 0x%08x \n", syn[0][k]);

            }
        }
    }

    // digit training 7
    inf_num <<= 1;
    for(int i = 0; i < N_7; i++){
        //printf("\n%d\n", i);
        for(int j = 0; j < ITE_PER_SAMPLE; j ++){
            //printf("%d\n", j);
            vin = 0;
            valid_spikes = 0;
            output = 0;
            syn_update = 0;
            dw = 0;
            for(int k =0; k < N_SYN64; k++){
                
                valid_spikes = ands(syn[7][k], spikes_7[i][j][k]);
                
                //printf("valid_spikes: 0x%08x \n", valid_spikes);

                vin = rpop(valid_spikes);

                //printf("vin: 0x%08x \n", vin);

                vneuron_update = sge(nadd(vneuron[7], vin), VTH);

                //printf("vneuron_update: 0x%08x \n", vneuron_update);

                vneuron[7] = vneuron_update;

                //printf("vneuron: 0x%08x \n", vneuron[0]);

                output = sls(output);
                
                //printf("%d\n", k);

                //printf("output: 0x%08x \n", output);
                inf(inf_num);
                syn_update = sup(syn[7][k],spikes_7[i][j][k]);

                //printf("syn_update: 0x%08x \n", syn_update);

                wsum = rpop(syn_update);

                //printf("wsum: 0x%08x \n", wsum);

                dw = wsum > WEXP ? wsum - WEXP : 0;
                possible = dw * 1024 / wsum;
                syn_update = ltd(possible, syn_update);

                //printf("syn_update: 0x%08x \n", syn_update);

                syn[7][k] = syn_update;

                //printf("syn 0x%08x \n", syn[0][k]);

            }
        }
    }

    // digit training 8
    inf_num <<= 1;
    for(int i = 0; i < N_8; i++){
        //printf("\n%d\n", i);
        for(int j = 0; j < ITE_PER_SAMPLE; j ++){
            //printf("%d\n", j);
            vin = 0;
            valid_spikes = 0;
            output = 0;
            syn_update = 0;
            dw = 0;
            for(int k =0; k < N_SYN64; k++){
                
                valid_spikes = ands(syn[8][k], spikes_8[i][j][k]);
                
                //printf("valid_spikes: 0x%08x \n", valid_spikes);

                vin = rpop(valid_spikes);

                //printf("vin: 0x%08x \n", vin);

                vneuron_update = sge(nadd(vneuron[8], vin), VTH);

                //printf("vneuron_update: 0x%08x \n", vneuron_update);

                vneuron[8] = vneuron_update;

                //printf("vneuron: 0x%08x \n", vneuron[0]);

                output = sls(output);
                
                //printf("%d\n", k);

                //printf("output: 0x%08x \n", output);
                inf(inf_num);
                syn_update = sup(syn[8][k],spikes_8[i][j][k]);

                //printf("syn_update: 0x%08x \n", syn_update);

                wsum = rpop(syn_update);

                //printf("wsum: 0x%08x \n", wsum);

                dw = wsum > WEXP ? wsum - WEXP : 0;
                possible = dw * 1024 / wsum;
                syn_update = ltd(possible, syn_update);

                //printf("syn_update: 0x%08x \n", syn_update);

                syn[8][k] = syn_update;

                //printf("syn 0x%08x \n", syn[0][k]);

            }
        }
    }

    // digit training 9
    inf_num <<= 1;
    for(int i = 0; i < N_9; i++){
        //printf("\n%d\n", i);
        for(int j = 0; j < ITE_PER_SAMPLE; j ++){
            //printf("%d\n", j);
            vin = 0;
            valid_spikes = 0;
            output = 0;
            syn_update = 0;
            dw = 0;
            for(int k =0; k < N_SYN64; k++){
                
                valid_spikes = ands(syn[9][k], spikes_9[i][j][k]);
                
                //printf("valid_spikes: 0x%08x \n", valid_spikes);

                vin = rpop(valid_spikes);

                //printf("vin: 0x%08x \n", vin);

                vneuron_update = sge(nadd(vneuron[9], vin), VTH);

                //printf("vneuron_update: 0x%08x \n", vneuron_update);

                vneuron[9] = vneuron_update;

                //printf("vneuron: 0x%08x \n", vneuron[0]);

                output = sls(output);
                
                //printf("%d\n", k);

                //printf("output: 0x%08x \n", output);
                inf(inf_num);
                syn_update = sup(syn[9][k],spikes_9[i][j][k]);

                //printf("syn_update: 0x%08x \n", syn_update);

                wsum = rpop(syn_update);

                //printf("wsum: 0x%08x \n", wsum);

                dw = wsum > WEXP ? wsum - WEXP : 0;
                possible = dw * 1024 / wsum;
                syn_update = ltd(possible, syn_update);

                //printf("syn_update: 0x%08x \n", syn_update);

                syn[9][k] = syn_update;

                //printf("syn 0x%08x \n", syn[0][k]);

            }
        }
    }
    for(int j = 0; j < N_NEU; j++){
        printf("syn of neuron 0{");
        for (int i = 0; i < N_SYN64; i++){
            if(i != N_SYN64 - 1){
                printf("%llu ", syn[0][i]);
            }
            else{
                printf("%llu}\n", syn[0][i]);
            }
        }
    }

    return 0;
}