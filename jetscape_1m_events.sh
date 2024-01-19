#!/bin/bash
#SBATCH --job-name=jetscape
#SBATCH --account=Project_2003154
#SBATCH --time=05:00:00
#SBATCH --mem-per-cpu=1800M
#SBATCH --nodes=1
#SBATCH --array=0-99
#SBATCH --partition=medium
#SBATCH --mail-user=maxim.m.o.virta@jyu.fi
#SBATCH --mail-type=BEGIN,END,FAIL


##SLURM_ARRAY_TASK_ID=0
##apptainer exec --bind ./output:/myJETSCAPE/JETSCAPE/build/output JETSCAPE /myJETSCAPE/JETSCAPE/build/runJetscape /myJETSCAPE/JETSCAPE/config/jetscape_user_PP19.xml
seq 0 127 | xargs -P127 -I{} apptainer exec --writable --bind ./output:/myJETSCAPE/JETSCAPE/build/output ../JETSCAPE \
sh -c "cd /myJETSCAPE/JETSCAPE/build/; ./runCustomJetscape ../config/jetscape_user_PP19.xml output/hepmc3Tree_${SLURM_ARRAY_TASK_ID}_{}.root;"
