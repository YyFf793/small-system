#include "game2048.h"

//数组初始化，将所有的图片保存在一个数组中:0~15
const char* pieces[] = {
	"standard/color_x80_2.jpg",
	"standard/color_x80_4.jpg",
	"standard/color_x80_8.jpg",
	"standard/color_x80_16.jpg",
	"standard/color_x80_32.jpg",
	"standard/color_x80_64.jpg",
	"standard/color_x80_128.jpg",
	"standard/color_x80_256.jpg",
	"standard/color_x80_512.jpg",
	"standard/color_x80_1024.jpg",
	"standard/color_x80_2048.jpg",
	"standard/color_x80_4096.jpg",
	"standard/color_x80_8192.jpg",
	"standard/color_x80_16384.jpg",
	"standard/color_x80_32768.jpg",
	"standard/color_x80_65536.jpg"
}; 

//2,棋盘矩阵的初始化
int matrix[5][5] = {0};
/*int matrix[4][4] = {
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0
};
*/

//根据要显示的数字来返回对应的文件名的下标
int get_piece_index(int x)
{
	if (x == 2) {
		return 0;
	}else if (x == 4) {
		return 1;
	}else if (x == 8) {
		return 2;
	}else if (x == 16) {
		return 3;
	}else if (x == 32) {
		return 4;
	}else if (x == 64) {
		return 5;
	}else if (x == 128) {
		return 6;
	}else if (x == 256) {
		return 7;
	}else if (x == 512) {
		return 8;
	}else if (x == 1024) {
		return 9;
	}else if (x == 2048) {
		return 10;
	}else if (x == 4096) {
		return 11;
	}else if (x == 8192) {
		return 12;
	}else if (x == 16384) {
		return 13;
	}else if (x == 32768) {
		return 14;
	}else if (x == 65536) {
		return 15;
	}
	
	return -1;
}

//4,求棋盘矩阵里面有多少个0
int get_zeros_count()
{
	int i, j;
	int count = 0;
	
	for (j = 0; j < 4; j++) {
		for (i = 0; i < 4; i++) {
			if (matrix[i][j] == 0) {
				count++;
			}
		}
	}
	
	return count;
}

//5,给棋盘矩阵第x个0的位置，填充一个值value
int set_matrix_value(int x, int value)
{
	int i, j;
	int num = 0;
	
	for (j = 0; j < 4; j++) {
		for (i = 0; i < 4; i++) {
			if (matrix[i][j] == 0) {
				num++;
				if (x == num) {
					matrix[i][j]  = value;
					return 0;
				}
			}
		}
	}
	
	return -1;
}

//6，触摸屏滑屏算法实现(判断方向1:左，2：右，3：上，4：下)
int get_finger_direction()
{
	struct input_event ts;
	int x1 = -1, y1 = -1;		//记录第一次滑动屏幕的位置
	int x2, y2, rt;					//不断地更新位置信息
	int x_read = 0, y_read = 1;
	
	while (1) {
		rt = read(ts_fd, &ts, sizeof(ts));
		if(rt != sizeof(ts)){
			continue;
		}
		//绝对坐标事件(x, y)  (y (y (x, (x, (y, fin up
		if (ts.type == EV_ABS) {
			if (ts.code == ABS_X && x_read == 0) {
				if (x1 == -1) {
					x1 = ts.value;
				}
				x2 = ts.value;
				x_read = 1;
				y_read = 0;
			}
			if (ts.code == ABS_Y && y_read == 0) {
				if (y1 == -1) {
					y1 = ts.value;
				}
				y2 = ts.value;
				x_read = 0;
				y_read = 1;
			}
		}
		
		//键盘事件
		if (ts.type == EV_KEY) {
			if (ts.code == BTN_TOUCH ) {
				if (ts.value == KEY_RESERVED) {
					int x_cz, y_cz;
					int abs_x, abs_y;
					
					x_cz = x2-x1;	//差值
					y_cz = y2-y1;
					abs_x = abs(x_cz); 	// a = |b|
					abs_y = abs(y_cz);
					
					if (x_cz > 30 && (abs_x > 2 * abs_y)) {
						return 1;
					}else if (x_cz < -30 && (abs_x > 2 * abs_y)) {
						return 2;
					}else if (y_cz > 30 && (abs_y > 2 * abs_x)) {
						return 3;
					}else if (y_cz < -30 && (abs_y > 2 * abs_x)) {
						return 4;
					}else {
						x1 = y1 = -1;
					}
					
				}else if (ts.value == KEY_ESC) {
					printf("fin press\n");
				}
			}
		}
	}
	
	return 0;
}

int lcd_draw_dect(int x, int y, int w, int h, int color)
{
	int i, j;
	
	for (j = 0; j < h; j++) {
		for (i = 0; i < w; i++) {
			lcd_draw_point(x+i, y+j, color);
		}
	}
	
	return 0;
}

/*
	draw_matrix:把棋盘矩阵在屏幕上显示出来
*/
void draw_matrix()
{
	int i, j;
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5;j++) {
			int x0, y0;
			x0 = 185;//棋盘矩阵左上角那个点的x轴坐标
			y0 = 25;//棋盘矩阵左上角那个点的y轴坐标
			if (matrix[i][j] == 0) {
				lcd_draw_dect(x0+j*90,  y0+i*90,  80, 80, 
						0x00ffffff);//如果此处元素的值为0，那么
								//就显示
			}else {
				int f_index = get_piece_index(matrix[i][j]);
				lcd_draw_jpg(x0+j*90, y0+i*90, pieces[f_index], NULL, 0, 0);
			}
		}
	}
}

/*
	init_matrix:初始化棋盘矩阵
			在任意x个位置，填充x个数字(2,4,8)
*/

void init_matrix()
{
	srandom(time(NULL));
	//规则x >= 1,x <= 3
	int x = (random() % 3) + 1;

	int i;

	/*
		step1:随机产生x个数字，并填充到棋盘矩阵中去
	*/

	for(i = 0; i < x; i++) {
		int pos = (random() % get_zeros_count()) + 1;

		int s[] = {2, 4, 8, 2};
		int s_i = (random() % 3);

		set_matrix_value(pos, s[s_i]);
	}

	/*
		step 2: 绘制棋盘矩阵
	*/
	draw_matrix();

}

/*
	rand1_matrix:移动之后随机产生一个数字填充到
	任意一个0的位置上
*/
void rand_matrix()
{
	srandom(time(NULL));
	int pos = (random() % get_zeros_count()) + 1;

	int s[] = {2, 4, 8, 2};
	int s_i = (random() % 4);

	set_matrix_value(pos, s[s_i]);
	draw_matrix();

}

/*
	fin_left:手指左划后棋子移动及合并的方式
*/
void fin_left()
{
	int i, j;//i为矩阵行下标，j为矩阵列下标
	int value, save_zero;
	for(i = 0; i < 5; i++){
		value = 0;
		save_zero= 0;
		for(j = 0; j < 5 ; j++){
			if (matrix[i][j] == 0) {
				continue;
			}
			
			if (value == 0) {
				value = matrix[i][j];
			}else{
				if (value == matrix[i][j]) {
					matrix[i][save_zero++] = value * 2;
					value = 0;
				} else {
					matrix[i][save_zero++] = value;
					value = matrix[i][j];
				}
			}
			matrix[i][j] = 0;
		}

		if (value != 0) {
			matrix[i][save_zero] = value;
		}

	}
		
}
	//draw_matrix();
/*
	fin_right:手指上划后棋子移动及合并的方式
*/
void fin_right()
{
	int i, j;//i为矩阵行下标，j为矩阵列下标
	int value;
	int save_zero;
	for (i = 0; i < 5; i++) {
		value = 0;
		save_zero = 5 -1;
		for (j = 5 - 1; j >= 0 ; j--) {
			if(matrix[i][j] == 0) {
				continue;
			}
			if(value == 0) {
				value = matrix[i][j];
			} else {
				if(value == matrix[i][j]) {
					matrix[i][save_zero--] = 2 * value;
					value = 0;
				} else {
					matrix[i][save_zero--] = value;
					value = matrix[i][j];
				}
			}
			matrix[i][j] = 0;
		}
		if(value != 0) {
			matrix[i][save_zero] = value;
		}
	
	}
}
/*
	fin_up:手指上划后棋子移动及合并的方式
*/
void fin_up()
{

	int i, j;//i为矩阵行下标，j为矩阵列下标
	int value;
	int save_zero;

	for(j = 0; j < 5; j++) {
		value = 0;
		save_zero= 0;
		for(i = 0; i < 5 ; i++) {
		
			if(matrix[i][j] == 0) {
				continue;
			}
			if(value == 0) {
				value = matrix[i][j];
			} else {
				if(value == matrix[i][j]) {
					matrix[save_zero++][j] =2 * value;
					value = 0;
				}else {
					matrix[save_zero++][j] = value;
					value = matrix[i][j];
				}
			}
			matrix[i][j] = 0;
		}
		if(value != 0) {
			matrix[save_zero][j] = value;
		}

	}

}
	
	//draw_matrix();
/*
	fin_down:手指上划后棋子移动及合并的方式
*/
void fin_down()
{
	int i, j;//i为矩阵行下标，j为矩阵列下标
	int value;
	int save_zero;
	
	for(j = 0; j < 5; j++) {
		value = 0;
		save_zero = 5 - 1;
		for(i = 5 - 1; i >= 0 ; i--) {
			if(matrix[i][j] == 0) {
				continue;
			}
			if(value == 0) {
				value = matrix[i][j];
			}else {
				if(value == matrix[i][j]) {
					matrix[save_zero--][j] = 2 * value;
					value = 0;
				}else {
					matrix[save_zero--][j] = value;
					value = matrix[i][j];
				}
			}
			matrix[i][j] = 0;
		}
		if(value != 0) {
			matrix[save_zero][j] = value;
		}
	}
	
}
/*
	move_judge:判断是否还能移动
	return value:
		1 game over
		0 continue
*/
int move_judge()
{
	int i, j;
	if(get_zeros_count() != 0){
		return 0;
	}

	for(i = 0; i < 5; i++) {
		for(j = 0; j < 5 ; j++) {
			if (j != 5 -1) {
				if (matrix[i][j] == matrix[i][j+1]) {
					return 0;
				}
			}

			if (i != 5 - 1) {
				if (matrix[i][j] == matrix[i+1][j]) {
					return 0;
				}
			}
		}
	}
	
	return 1;

}


/*
	change_matrix:根据手指滑动(direction)，
		变换棋盘矩阵
*/
int change_matrix()
{
	
	int direction = get_finger_direction();
	if (direction == 2) {
		fin_left();
	}else if (direction == 1) {
		fin_right();
	}else if (direction == 3) {
		fin_up();
	}else if (direction == 4) {
		fin_down();
	}
}


int game2048()
{
    int rt;

	lcd_draw_jpg(0, 0, "game2048.jpg", NULL, 0, 0);
	
	//将时间设置为随机数种子
	srandom(time(NULL));
	init_matrix();
	
	
	while (1) {
		int matrix_v2[5][5];
		int i, j, flag = 0;
		for (i = 0; i < 5; ++i) {
			for (j = 0; j < 5; ++j) {
				matrix_v2[i][j] = matrix[i][j];
			}
		}
		
		//变换矩阵
		change_matrix();
		
		for (i = 0; i < 5; ++i) {
			for (j = 0; j < 5; ++j) {
				if (matrix_v2[i][j] != matrix[i][j]) {
					flag = 1;
					i = j = 5;
				}
			}
		}
		
		if (flag) {
			rand_matrix();
			draw_matrix();
		}else {
			draw_matrix();
		}
		
		//判断游戏是否结束
		rt = move_judge();
		if (rt == 1) {
			break;
		}
    }

    lcd_draw_jpg(250, 165, "standard/game_over_300x150.jpg", NULL, 0, 0);
	
	return 0;
}


