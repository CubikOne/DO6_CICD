#!/bin/bash

S21_CAT_PATH="/home/gitlab-runner/builds/zvqqzwqc/0/students/DO6_CICD.ID_356283/ironmakm_student.21_school.ru/DO6_CICD-1/src/C2_SimpleBashUtils/cat/s21_cat"
S21_GREP_PATH="/home/gitlab-runner/builds/zvqqzwqc/0/students/DO6_CICD.ID_356283/ironmakm_student.21_school.ru/DO6_CICD-1/src/C2_SimpleBashUtils/grep/s21_grep"

DEPLOY_USER="deploy"
DEPLOY_HOST="192.168.0.12"

chmod +x $S21_CAT_PATH $S21_GREP_PATH

scp $S21_CAT_PATH $DEPLOY_USER@$DEPLOY_HOST:/usr/local/bin
scp $S21_GREP_PATH $DEPLOY_USER@$DEPLOY_HOST:/usr/local/bin
