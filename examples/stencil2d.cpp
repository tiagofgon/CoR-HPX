/*
    Tiago Gonçalves & António Pina, UM - LIP, 2021

    To run: 
        With MPI: mpirun -np N ./corhpx apps ctx N 0 ../examples/libstencil2d.so
*/


#include "cor/cor.hpp"


std::shared_ptr<cor::Operon_Client> operon;
typedef hpx::lcos::channel<std::vector<double>> channel_type;


void worker(std::shared_ptr<cor::Domain_Client> domain, std::size_t global_rank, std::size_t num, std::size_t Nx, std::size_t Ny, std::size_t steps)
{

    auto rank = global_rank + operon->GetRank() - 1;

    std::cout << "rank " << rank << std::endl;
    std::cout << "Nx " << Nx << std::endl;
    std::cout << "Ny " << Ny << std::endl;
    std::cout << "steps " << steps << std::endl;



    typedef std::vector<double> data_type;
    std::array<data_type, 2> U;

    U[0] = data_type(Nx * Ny, 0.0);
    U[1] = data_type(Nx * Ny, 0.0);


    // Initialize: Boundaries are set to 1, interior is 0
    // linha de cima
    if (rank == 0)
    {
        std::fill(U[0].begin(), U[0].begin() + Nx, 1.0);
        std::fill(U[1].begin(), U[1].begin() + Nx, 1.0);
    }
    // meio
    for (std::size_t y = 0; y < Ny; ++y)
    {
        U[0][y * Nx + 0] = 1.0;
        U[1][y * Nx + 0] = 1.0;

        U[0][y * Nx + (Nx - 1)] = 1.0;
        U[1][y * Nx + (Nx - 1)] = 1.0;
    }
    // linha de baixo
    if (rank == num - 1)
    {
        std::fill(U[0].end() - Nx, U[0].end(), 1.0);
        std::fill(U[1].end() - Nx, U[1].end(), 1.0);
    }

    // for(std::size_t i = 0; i < Ny; i++)
    // {
    //     for(std::size_t j = 0; j < Nx; j++)
    //     {
    //         std::cout << U[0][i*Nx + j] << " ";
    //     }
    //     std::cout << std::endl;
    // }std::cout << std::endl;



    std::string myself = "rank" + std::to_string(rank);
    std::string upper_neighbor = "";
    std::string bottom_neighbor = "";

    std::unique_ptr<cor::MultiChannel_Client<std::vector<double>>> myswitch;

    if (num > 1)
    {
        // We have an upper and bottom neighbors
        if ((rank > 0) && (rank < num - 1))
        {
            // std::cout << "tenho vizinho de cima e de baixo" << std::endl;
            upper_neighbor = "rank" + std::to_string(rank-1);
            bottom_neighbor = "rank" + std::to_string(rank+1);
            myswitch = std::move(domain->CreateLocal<cor::MultiChannel_Client<std::vector<double>>>(domain->Idp(), "",  myself, upper_neighbor, bottom_neighbor));

            // send initial value to our upper and bottom neighbors
            myswitch->Set(std::vector<double>(U[0].begin(), U[0].begin() + Nx), upper_neighbor, 0);
            myswitch->Set(std::vector<double>(U[0].end() - Nx, U[0].end()), bottom_neighbor, 0);
        }
        // We have an upper neighbor, only
        else if (rank > 0) 
        {
            // std::cout << "tenho vizinho de cima" << std::endl;
            upper_neighbor = "rank" + std::to_string(rank-1);
            myswitch = std::move(domain->CreateLocal<cor::MultiChannel_Client<std::vector<double>>>(domain->Idp(), "",  myself, upper_neighbor));

            // send initial value to our upper neighbor
            myswitch->Set(std::vector<double>(U[0].begin(), U[0].begin() + Nx), upper_neighbor, 0);
        }
        // We have an bottom neighbor, only
        else if (rank < num - 1)
        {
            // std::cout << "tenho vizinho de baixo" << std::endl;
            myself = "rank" + std::to_string(rank);
            bottom_neighbor = "rank" + std::to_string(rank+1);
            myswitch = std::move(domain->CreateLocal<cor::MultiChannel_Client<std::vector<double>>>(domain->Idp(), "",  myself, bottom_neighbor));

            // send initial value to our neighbor below
            myswitch->Set(std::vector<double>(U[0].end() - Nx, U[0].end()), bottom_neighbor, 0);
        }
    }



    using namespace hpx::execution;
    typedef hpx::compute::host::block_executor<> executor_type;
    auto numa_domains = hpx::compute::host::numa_domains();
    executor_type executor(numa_domains);
    auto policy = par(task).on(executor);


    hpx::chrono::high_resolution_timer t;

    hpx::future<void> step_future = hpx::make_ready_future();
    for (std::size_t t = 0; t < steps; ++t) {

        // step_future = step_future.then([&U, &myswitch, upper_neighbor, bottom_neighbor, Ny, Nx, t, policy](hpx::future<void>&& prev) mutable
        // {

            hpx::future<void> top_boundary_future;
            if (upper_neighbor != "") {
                // std::cout << "vizinho_cima" << std::endl;
                top_boundary_future = myswitch->Get(upper_neighbor, t).then([&U, &myswitch, upper_neighbor, Nx, Ny, t](hpx::future<std::vector<double>>&& up_future){
                    std::vector<double> up = up_future.get();

                    // Iterate over the interior: skip the last and first element
                    for(int j = 1; j < Nx-1; j++)
                    {
                        U[1][j] = 0.25 * (up[j-1] + up[j+1] + U[0][(j + Nx) - 1] + U[0][(j + Nx) + 1]) - U[0][j];
                    }

                    std::vector<double> newVec(U[1].begin(), U[1].begin()+Nx);
                    myswitch->Set(std::move(newVec), upper_neighbor, t + 1);
                    // std::cout << "vizinho_cima enviou" << std::endl;
                });

            }
            else {
                top_boundary_future = hpx::make_ready_future();
            }



            // // Update our interior spatial domain
            // hpx::future<void> interior_future = hpx::async([&U, Nx, Ny](){
            //     // Iterate over the interior: skip the last and first element
            //     for(std::size_t i = 1; i < Ny-1; ++i)
            //     {
            //         for(std::size_t j = 1; j < Nx-1; ++j)
            //         {
            //             U[1][i*Nx + j] = 0.25 * (U[0][(i-1)*Nx + j-1] + U[0][(i-1)*Nx + j+1] + U[0][(i+1)*Nx + j-1] + U[0][(i+1)*Nx + j+1]) - U[0][i*Nx + j];
            //         }
            //     }
            // });


            hpx::future<void> interior_future = hpx::for_loop(policy, 1, Ny-1,
                [&U, Nx, Ny](std::size_t i) {
                    for(std::size_t j = 1; j < Nx-1; ++j)
                    {
                        U[1][i*Nx + j] = 0.25 * (U[0][(i-1)*Nx + j-1] + U[0][(i-1)*Nx + j+1] + U[0][(i+1)*Nx + j-1] + U[0][(i+1)*Nx + j+1]) - U[0][i*Nx + j];
                    }

                });
            



            hpx::future<void> bottom_boundary_future;
            if (bottom_neighbor != "")
            {
                // std::cout << "vizinho_baixo" << std::endl;
                bottom_boundary_future = myswitch->Get(bottom_neighbor, t).then([&U, &myswitch, bottom_neighbor, Nx, Ny, t](hpx::future<std::vector<double>>&& bottom_future){
                    std::vector<double> down = bottom_future.get();

                    // Iterate over the interior: skip the last and first element
                    for(int j = 1; j < Nx-1; j++)
                    {
                        U[1][(Ny-1)*Nx + j] = 0.25 * (U[0][(Ny-2)*Nx + j-1] + U[0][(Ny-2)*Nx + j+1] + down[j-1] + down[j+1]) - U[0][(Ny-1)*Nx + j];
                    }

                    std::vector<double> newVec(U[1].end() - Nx, U[1].end());
                    myswitch->Set(std::move(newVec), bottom_neighbor, t + 1);
                    // std::cout << "vizinho_baixo enviou" << std::endl;
                });
            }
            else {
                bottom_boundary_future = hpx::make_ready_future();
            }

        
            //return hpx::when_all(top_boundary_future, interior_future, bottom_boundary_future);

            hpx::wait_all(top_boundary_future, interior_future, bottom_boundary_future);

        // });

        U[0].swap(U[1]);
    }
    
    // step_future.get();
    double elapsed = t.elapsed();


    if (rank == 0)
    {
        double mlups = (((Nx - 2.) * (Ny * num - 2.) * steps) / 1e6)/ elapsed;
        std::cout << "MLUPS: " << mlups << "\n";


        // std::ofstream file("stencil2d_output");
        // for(std::size_t y = 0; y != Ny; ++y)
        // {
        //     for(std::size_t x = 0; x != Nx; ++x)
        //     {
        //         file << x << " " << y << " " << U[0][y * Nx + x] << '\n';
        //     }
        // }
    }


    // for(std::size_t i = 0; i < Ny; i++)
    // {
    //     for(std::size_t j = 0; j < Nx; j++)
    //     {
    //         std::cout << U[0][i*Nx + j] << " ";
    //     }
    //     std::cout << std::endl;
    // }std::cout << std::endl;





}

extern "C"
{
    void Main(int argc, char *argv[]);
}


/*
argv[0] == Nx
argv[1] == Ny_global
argv[2] == steps
argv[3] == num_local_partitions
*/
void Main(int argc, char *argv[]) {
    auto domain = cor::GetDomain();
    auto domain_idp = domain->Idp();
    auto agent_idp = domain->GetActiveResourceIdp();
    auto agent = domain->GetLocalResource<cor::ProtoAgent_Client<void(char**)>>(agent_idp);
    auto clos_idp = domain->GetPredecessorIdp(agent_idp);
    auto clos = domain->GetLocalResource<cor::Closure_Client>(clos_idp);
    auto rank = clos->GetIdm(agent_idp);

    
    std::size_t Nx = 1024; // numero de colunas
    std::size_t Ny_global = 1024; // numero de linhas totais
    std::size_t steps = 100; // numero de steps
    std::size_t num_local_partitions = 1; // numero de particoes locais

    if(argc >= 1) {
        Nx = atoi(argv[0]);
    };
    if (argc >= 2){
        Ny_global = atoi(argv[1]);
    };
    if (argc >= 3) {
        steps = atoi(argv[2]);
    };
    if (argc >= 4) {
        num_local_partitions = atoi(argv[3]);
    };

    std::size_t locality = hpx::get_locality_id(); // rank desta localidade
    std::size_t num_localities = hpx::get_num_localities(hpx::launch::sync); // numero de localidades

    // std::cout << "Nx " << Nx << std::endl;
    // std::cout << "Ny_global " << Ny_global << std::endl;
    // std::cout << "steps " << steps << std::endl;

    std::size_t num_partitions = num_localities * num_local_partitions; // o numero total de particoes = numeros de localidades * particoes locais

    // We divide our grid in stripes along the y axis.
    std::size_t Ny = Ny_global / num_partitions; // dividir blocos de linhas para cada localidade



    operon = domain->CreateLocal<cor::Operon_Client>(domain->Idp(),  "", num_local_partitions);
    auto res = operon->Dispatch(&worker, domain, (locality * num_local_partitions), num_partitions, Nx, Ny, steps);
    res.get();


    std::cout << "Job finnished" << std::endl;

}