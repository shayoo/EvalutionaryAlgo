#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphs.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_FPS_clicked()
{

    //initial graph
    graphs gr;
    gr.setModal(true);

    int k = 0;
    while(k < COUNT){

        TotalFitness = 0;
        population.clear();
        newpopulation.clear();

        generatePopulations(false);

        //sort by fitness
        sort(this->population.begin(), this->population.end(), by_fitness());

        int j = 0;
        while(j < COUNT){

            calc_prob_ranges(false);

            //offsprings
            for(int i=0; i<((COUNT/2)/2); i++){
                float val = randomRange(0, 1);
                temp = generateOffspring(val);

                offsprint_x1 = temp.x;
                offsprint_y1 = temp.y;

                val = randomRange(0, 1);
                temp = generateOffspring(val);

                offsprint_x2 = temp.x;
                offsprint_y2 = temp.y;

                //mutation process
                mutation(offsprint_x1, offsprint_y2, offsprint_x2, offsprint_y1, false);
            }

            //merge new with old population
            population.insert(population.end(), newpopulation.begin(), newpopulation.end());

            //clear new population
            newpopulation.clear();

            //sort by compute
            sort(population.begin(), population.end(), by_fitness());

            // erase unecessary elements/population:
            population.erase(population.begin()+COUNT,population.end());

            //cout << "population for new iteration" << endl;
            TotalFitness = 0;
            for(int i=0; i < population.size(); i++ ){
                TotalFitness = TotalFitness + population[i].fitness;
                //cout << population[i].x << "," << population[i].y << "," << population[i].fitness << endl;
            }

            //best of iteration
            best.push_back(population[0].fitness);
            //cout << "fps best" << population[0].fitness << endl;
            //avg of iteration
            avg.push_back(population[(COUNT/2)-1].fitness);

            j++;

        }
        k++;
        TotalFitness = 0;
        pso();
        ais();
        gr.data.push_back({best, avg, pso_best, ais_best, ais_avg,COUNT});
        reverse(pso_best.begin(), pso_best.end());
        reverse(ais_best.begin(), ais_best.end());
        reverse(ais_avg.begin(), ais_avg.end());
        reverse(best.begin(), best.end());
        reverse(avg.begin(), avg.end());
        best_best.push_back(best[0]);
        avg_avg.push_back(avg[0]);
        ais_avg_avg.push_back(ais_avg[0]);
        pso_bb.push_back(pso_best[0]);
        pso_best.clear();
        ais_bb.push_back(ais_best[0]);
        ais_best.clear();
        ais_avg.clear();
        best.clear();
        avg.clear();
    }

    gr.data.push_back({best_best,avg_avg, pso_bb, ais_bb, ais_avg_avg,COUNT});
    best_best.clear();
    avg_avg.clear();
    ais_bb.clear();
    pso_bb.clear();
    ais_avg_avg.clear();

//    for(int i=0; i < COUNT; i++ ){
//        gr.best.push_back({best[i].fitness});
//        gr.avg.push_back({avg[i].fitness});
//    }
//        cout << gr.best.size() << endl;

//    gr.generation = COUNT;


    gr.makePlot(0);

    gr.exec();

}


void MainWindow::on_RBS_clicked()
{


    //initial graph
//    graphs gr;
//    gr.setModal(true);

//    int k = 0;
//    while(k < COUNT){

//        TotalFitness = 0;
//        population.clear();
//        newpopulation.clear();

//        generatePopulations(true);

//        //sort by fitness
//        sort(this->population.begin(), this->population.end(), by_x());

//        int j = 0;
//        while(j < COUNT){

//            calc_prob_ranges(true);

//            //offsprings
//            for(int i=0; i<((COUNT/2)/2); i++){
//                float val = randomRange(0, 1);
//                temp = generateOffspring(val);

//                offsprint_x1 = temp.x;
//                offsprint_y1 = temp.y;

//                val = randomRange(0, 1);
//                temp = generateOffspring(val);

//                offsprint_x2 = temp.x;
//                offsprint_y2 = temp.y;

//                //mutation process
//                mutation(offsprint_x1, offsprint_y2, offsprint_x2, offsprint_y1, true);
//            }

//            //merge new with old population
//            population.insert(population.end(), newpopulation.begin(), newpopulation.end());

//            //clear new population
//            newpopulation.clear();

//            //sort by compute
//            sort(population.begin(), population.end(), by_fitness());

//            // erase unecessary elements/population:
//            population.erase(population.begin()+COUNT,population.end());

//            cout << "population for new iteration" << endl;
//            TotalFitness = 0;
//            for(int i=0; i < population.size(); i++ ){
//                TotalFitness = TotalFitness + population[i].x;
//                //cout << population[i].x << "," << population[i].y << "," << population[i].fitness << endl;
//            }

//            //best of iteration
//            best.push_back(population[0].fitness);
//            //avg of iteration
//            avg.push_back(population[(COUNT/2)-1].fitness);

//            j++;

//        }
//        k++;
//        gr.data.push_back({best,avg,COUNT});
//        reverse(best.begin(), best.end());
//        reverse(avg.begin(), avg.end());
//        best_best.push_back(best[0]);
//        avg_avg.push_back(avg[0]);
//        best.clear();
//        avg.clear();
//    }

//    gr.data.push_back({best_best,avg_avg,COUNT});
//    best_best.clear();
//    avg_avg.clear();

////    for(int i=0; i < COUNT; i++ ){
////        gr.best.push_back({best[i].fitness});
////        gr.avg.push_back({avg[i].fitness});
////    }
////        cout << gr.best.size() << endl;

////    gr.generation = COUNT;


//    gr.makePlot(0);

//    gr.exec();
}


float MainWindow::randomRange(float min, float max){
//    return (min + (rand() % (int)(max - min + 1)));
    //return (min + (rand() % (max - min + 1)));
    float r3 = min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
    return r3;
}

float MainWindow::getFitness(float x, float y, bool isRBS){
     //float ans = fabs((2*x)+(3*y));
    float ans = (pow (x, 2))+(pow (y, 2));
    //ans = round(ans, 3);
    if(isRBS)
     TotalFitness = TotalFitness + x;
    else
     TotalFitness = TotalFitness + ans;

     return ans;
}


void MainWindow::mutation(float x1, float y2, float x2, float y1, bool isRBS){

    float val, fit1, fit2;

    fit1 = getFitness(x1, y2, isRBS);
    fit2 = getFitness(x2, y1, isRBS);

    //for x1
    if(x1 > 0.2){
//    val = randomRange(0, 1);
//    if(val > 0.2){
        val = randomRange(-0.5, 0.5);
        if(val+x1 > MAXX)
            x1 = MAXX;
        else if(val+x1 < MINX)
            x1 = MINX;
        else
            x1 = val+x1;
    }

    //for y2
    if(y2 > 0.2){
//    val = randomRange(0, 1);
//    if(val > 0.2){
        val = randomRange(-0.5, 0.5);
        if(val+y2 > MAXY)
            y2 = MAXY;
        else if(val+y2 < MINY)
            y2 = MINY;
        else
            y2 = val+y2;
    }

    //for x2
    if(x2 > 0.2){
//    val = randomRange(0, 1);
//    if(val > 0.2){
        val = randomRange(-0.5, 0.5);
        if(val+x2 > MAXX)
            x2 = MAXX;
        else if(val+x2 < MINX)
            x2 = MINX;
        else
            x2 = val+x2;
    }

    //for y1
    if(y1 > 0.2){
//    val = randomRange(0, 1);
//    if(val > 0.2){
        val = randomRange(-0.5, 0.5);
        if(val+y1 > MAXY)
            y1 = MAXY;
        else if(val+y1 < MINY)
            y1 = MINY;
        else
            y1 = val+y1;
    }

//    newpopulation.push_back({x1, y2, fit1});
//    newpopulation.push_back({x2, y1, fit2});

    newpopulation.push_back({x1, y2, getFitness(x1, y2, isRBS)});
    newpopulation.push_back({x2, y1, getFitness(x2, y1, isRBS)});

}



void MainWindow::generatePopulations(bool isRBS){
    for(int i = 0; i < COUNT; i++){
        float x = randomRange(MINX, MAXX);
        float y = randomRange(MINY, MAXY);
        this->population.push_back({x, y, getFitness(x, y, isRBS)});
        //cout << this->population[i].fitness << endl;
    }
}

void MainWindow::calc_prob_ranges(bool isRBS){
    //cout << "probility & range" << endl;
    float range = 0;
    for(int i=0; i < COUNT; i++ ){
        //probility
        if(isRBS)
            probilities[i] = this->population[i].x/TotalFitness;
        else
            probilities[i] = this->population[i].fitness/TotalFitness;
        //probility range
        range = range + probilities[i];
        probilities_ranges[i] = range;
        //cout << range << endl;
    }
}

//http://www.mnemstudio.org/particle-swarm-introduction.htm
//http://www.swarmintelligence.org/tutorials.php
void MainWindow::pso(void){

    //initialize particles
    this->gbest = {{0, 0}, 0};
    for(int i = 0; i < COUNT; i++){
        float x = randomRange(MINX, MAXX);
        float y = randomRange(MINY, MAXY);
        this->particle.push_back({{x, y}, NULL});
        this->lbest.push_back({{0, 0}, 0});
        this->present.push_back({{0, 0}, 0});
        //cout << "particle x:" << this->v[i].x << "particle y:" << this->v[i].y << endl;
    }


    //    While maximum iterations or minimum error criteria is not attained
    int k = 0;
    while(k < COUNT){

        for(int i = 0; i < COUNT; i++){
//            Calculate Data fitness value
              this->present[i].fitness = getFitness(this->present[i].v.x, this->present[i].v.y, false);
//            If the fitness value is better than pBest
//            {
//                Set pBest = current fitness value
//            }
              if(this->present[i].fitness > lbest[i].fitness){
//                  cout << "change in lbest" << endl;
                  lbest[i] = this->present[i];
              }
//            If pBest is better than gBest
//            {
//                Set gBest = pBest
//            }
              if(lbest[i].fitness > gbest.fitness){
//                  cout << "change in gbest" << endl;
                  gbest = lbest[i];
              }
        }

        cout << "gbest" << gbest.fitness << endl;
        pso_best.push_back(gbest.fitness);

        //problem values obtaining is bigger than limit
        for(int i = 0; i < COUNT; i++){
    //            Calculate particle Velocity
//            cout << "old v.x" << v[i].x << endl;
              particle[i].v.x = particle[i].v.x + C1 * randomRange(0, 1) * (gbest.v.x - present[i].v.x) + C2 * randomRange(0, 1) * (lbest[i].v.x - present[i].v.x);
              particle[i].v.y = particle[i].v.y + C1 * randomRange(0, 1) * (gbest.v.y - present[i].v.y) + C2 * randomRange(0, 1) * (lbest[i].v.y - present[i].v.y);

              if(particle[i].v.x < MINX)
                  particle[i].v.x = MINX;
              else if(particle[i].v.x > MAXX)
                  particle[i].v.x = MAXX;

              if(particle[i].v.y < MINY)
                  particle[i].v.y = MINY;
              else if(particle[i].v.y > MAXY)
                  particle[i].v.y = MAXY;

//              cout << "new particle.v.x" << particle[i].v.x << "new particle.v.y" << particle[i].v.x << endl;
    //            Use gBest and Velocity to update particle Data
              present[i].v.x = present[i].v.x + particle[i].v.x;
              present[i].v.y = present[i].v.y + particle[i].v.y;

              if(present[i].v.x < MINX)
                  present[i].v.x = MINX;
              else if(present[i].v.x > MAXX)
                  present[i].v.x = MAXX;

              if(present[i].v.y < MINY)
                  present[i].v.y = MINY;
              else if(present[i].v.y > MAXY)
                  present[i].v.y = MAXY;
        }


        k++;
    }


}


void MainWindow::ais(void){

    TotalFitness = 0;
    ais_population.clear();

    for(int i = 0; i < COUNT; i++){
        float x = randomRange(MINX, MAXX);
        float y = randomRange(MINY, MAXY);
        this->ais_population.push_back({x, y, getFitness(x, y, false)});
        //cout << this->population[i].fitness << endl;
    }

    //sort by fitness
    sort(this->ais_population.begin(), this->ais_population.end(), by_fitness());

    int j = 0;
    while(j < COUNT){

        //clone half of population
       vector<data> ais_newpopulation(this->ais_population.begin(),(this->ais_population.begin()+(COUNT/2)));

        //mutation
        for(int i = 0; i < (COUNT/2); i++){
            float mutate = randomRange(-0.05, 0.05);
            if(mutate < 0)
                ais_newpopulation[i].fitness = ais_newpopulation[i].fitness + ((1/ais_newpopulation[i].fitness) * mutate);
            else
                ais_newpopulation[i].fitness = ais_newpopulation[i].fitness + ((1/ais_newpopulation[i].fitness) * mutate);
    //        this->ais_population.push_back({x, y, getFitness(x, y, false)});

        }

        for(int i = 0; i < 3; i++){
            float x = randomRange(MINX, MAXX);
            float y = randomRange(MINY, MAXY);
            ais_newpopulation.push_back({x, y, getFitness(x, y, false)});
            //cout << this->population[i].fitness << endl;
        }

        //merge new with old population
        ais_population.insert(ais_population.end(), ais_newpopulation.begin(), ais_newpopulation.end());

        //sort by fitness
        sort(ais_population.begin(), ais_population.end(), by_fitness());

        // erase unecessary elements/population:
        ais_population.erase(ais_population.begin()+COUNT,ais_population.end());

        //best
        ais_best.push_back(ais_population[0].fitness);

        //best
        ais_avg.push_back(ais_population[COUNT/2].fitness);

        j++;
        TotalFitness = 0;
    }

}
