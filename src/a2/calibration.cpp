#include "calibration.hpp"
#include <vector>
#include <fstream>
#include <iostream>
#include "hsv.hpp"
#include "math/matd.h"
#include "math/Point.hpp"

calibration_t::calibration_t(){
    tx_mat = matd_create(3,3);
}

calibration_t::~calibration_t(){
    matd_destroy(tx_mat);
}

void calibration_t::read_tx_mat(char* filename){
    FILE *fp = fopen(filename,"r");
    double e[6];
    if(fp != NULL){
        for(int i=0;i<6;++i){
            fscanf(fp,"%f\n",&e[i]);
        }
        matd_put(tx_mat, 0, 0, e[0]);
        matd_put(tx_mat, 0, 1, e[1]);
        matd_put(tx_mat, 0, 2, e[2]);
        matd_put(tx_mat, 1, 0, e[3]);
        matd_put(tx_mat, 1, 1, e[4]);
        matd_put(tx_mat, 1, 2, e[5]);
        matd_put(tx_mat, 2, 0, 0);
        matd_put(tx_mat, 2, 1, 0);
        matd_put(tx_mat, 2, 2, 1.0); 
    }
    else{
        printf("Error on opening matrix file\n");
        exit(1);
    }
}

void calibration_t::read_red_range(char* filename){
    red_range.read_hsv_from_file(filename);
}

void calibration_t::read_cyan_range(char *filename){
    cyan_range.read_hsv_from_file(filename);
}

void calibration_t::read_green_range(char *filename){
    green_range.read_hsv_from_file(filename);
}

void calibration_t::read_mask(char* filename){
    FILE *fp = fopen("../calibration/mask_rect.txt","r");
    float e[4];
    if(fp != NULL){
        fscanf(fp,"%f %f %f %f\n",&e[0],&e[1].&e[2],&e[3]);
        mask[0] = e[0];
        mask[1] = e[1];
        mask[2] = e[2];
        mask[3] = e[3];
    }
    else{
        printf("Error on opening mask file\n");
        exit(1);
    } 
}

void calibration_t::read_default_all(){
    read_tx_mat("transform_elements.txt");
    read_mask("mask_rect.txt");
    read_cyan_range("cyan_hsv_range.txt");
    read_red_range("red_hsv_range.txt");
    read_green_range("green_hsv_range.txt");
}

max_min_hsv calibration_t::get_cyan(){
    return cyan_range;
}

max_min_hsv calibration_t::get_green(){
    return green_range;
}

max_min_hsv calibration_t::get_red(){
    return red_range;
}

std::vector<float> calibration_t::get_mask(){
    return mask;
}

eecs467::Point<double> calibration_::translate(eecs467::cam_coords){
    eecs467::Point<double> ground_in_arm;
    ground_in_arm.x = 
            matd_get(tx_mat, 0, 0)*cam_coords.x
            + matd_get(tx_mat, 0, 1)*cam_coords.y 
            + matd_get(tx_mat, 0, 2)*1.0;
    ground_in_arm.y = 
            matd_get(tx_mat, 1, 0)*cam_coords.x 
            + matd_get(tx_mat, 1, 1)*cam_coords.y 
            + matd_get(tx_mat, 1, 2)*1.0;
    return ground_in_arm; 
}


