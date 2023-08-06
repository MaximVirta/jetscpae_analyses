#!/bin/bash
#SBATCH --job-name=jetscape
#SBATCH --account=Project_2003154
#SBATCH --time=05:00:00
#SBATCH --mem-per-cpu=1800M
#SBATCH --nodes=1
#SBATCH --array=0-99
#SBATCH --partition=medium

##SLURM_ARRAY_TASK_ID=0
##apptainer exec --bind ./output:/myJETSCAPE/JETSCAPE/build/output JETSCAPE /myJETSCAPE/JETSCAPE/build/runJetscape /myJETSCAPE/JETSCAPE/config/jetscape_user_PP19.xml
apptainer exec --bind ./output:/myJETSCAPE/JETSCAPE/build/output,./input:/myJETSCAPE/JETSCAPE/build/input ../JETSCAPE \
sh -c "cd /myJETSCAPE/JETSCAPE/build/; ./runJetscape /myJETSCAPE/JETSCAPE/build/input/jetscape_${SLURM_ARRAY_TASK_ID}_PP19.xml;"
