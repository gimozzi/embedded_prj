package com.example.androidex;

import java.util.Random;

import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;

//import android.view.ViewGroup.LayoutParams;

public class MainActivity2 extends Activity {

	// puzzle xml Button
	Button btn_make_buttons;

	// Button puzzle[]; // 2*2 < , <= 5*5 (Not 1*1, 1*2, 2*1)
	// Button puzzle[][];
	Button[][] puzzle;
	int[][] puzzle_array;

	int row, col;
	int black_row, black_col;
	boolean input_flag;

	// onCreate
	LinearLayout linear;

	protected void onCreate(Bundle savedInstanceState) {
		// ctrl + shit + o 하면 import 됨
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main2);
		linear = (LinearLayout) findViewById(R.id.container);

		// puzzle code start
		btn_make_buttons = (Button) findViewById(R.id.id_create_puzzle);

		linear.setOrientation(LinearLayout.VERTICAL); // p20

		Log.d("MY_TAG", "Activity2 onCreate");
		// touch Make button
		OnClickListener listener = new OnClickListener() {
			// Make buttons 눌렀을 때
			@Override
			public void onClick(View v) {
				EditText text_input;
				text_input = (EditText) findViewById(R.id.id_input);

				String read_Edittext;
				String tokens[];
				read_Edittext = text_input.getText().toString();

				input_flag = false;
				row = 0;
				col = 0;

				// Edittext 에서 읽은 문자열 error check
				if (read_Edittext.length() != 0) {
					tokens = read_Edittext.split(" "); // 공백으로 구분
					if (tokens.length == 2) {
						Log.d("MY_TAG", "before parse");
						try {
							row = Integer.parseInt(tokens[0]);
							col = Integer.parseInt(tokens[1]);
						} catch (Exception e) {
							input_flag = false;
						}
						Log.d("MY_TAG", "integer?");
						if ((row >= 1 && row <= 5) && (col >= 1 && col <= 5)) {
							input_flag = true;
							// 1*1, 1*2, 2*1 안됨
						} else {
							input_flag = false;
						}
						Log.d("MY_TAG", "integer");
					}
				} else {
					input_flag = false;
				}

				// 정확한 input 이 들어온 경우 make buttons 동작 수행
				if (input_flag) {
					Log.d("MY_TAG", "input_flag : true");
					// arrays
					// puzzle = new Button[row * col];
					puzzle = new Button[row][col];
					// puzzle_array = new int[row * col];
					puzzle_array = new int[row][col];
					for (int i = 0, number = 1; i < row; i++) {
						for (int j = 0; j < col; j++) {
							// puzzle_array[i*col + j] = number++;
							puzzle_array[i][j] = number++;
						}
					}

					// mix buttons
					Log.d("MY_TAG", "mix buttons");
					mix_array();

					// check_puzzle
					check_puzzle(); // 생성된 퍼즐이 되는 일이 없는지 check:
					// 1*2 같이 가능은 하지만 랜덤 생성이 바로 종료조건으로 생성되는 경우 초기화면으로 돌아간다
					// 빈칸 1 과 같이 생성되면 그대로 오락 진행

					// create buttons
					make_buttons_dynamically();
					Log.d("MY_TAG", "버튼생성 완료");

					// solve puzzle
					
					for (int i = 0; i < row; i++) {
						for (int j = 0; j < col; j++) {
							solve_puzzle(puzzle[i][j]);
						}
					}

				} else {
					Log.d("MY_TAG", "back");
					onBackPressed();
				}

			}
		};
		btn_make_buttons.setOnClickListener(listener);
		// End of onCreate()
	}

	// Making buttons dynamically
	public void make_buttons_dynamically() {
		Log.d("MY_TAG", "call make_buttons_dynamically");

		for (int i = 0; i < row; i++) {
			LinearLayout b_row = new LinearLayout(this);
			b_row.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT, 1));


			// import android.view.ViewGroup.LayoutParams;
			b_row.setId(1000 + i);

			for (int j = 0; j < col; j++) {
				Button btnTag = new Button(this);
				btnTag.setLayoutParams(new LayoutParams(
						LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT, 1));
			
				btnTag.setText(String.valueOf(puzzle_array[i][j]));
				btnTag.setId(j + 1 + (i * col));

				if (puzzle_array[i][j] == row * col) {
					btnTag.setBackgroundColor(Color.BLACK);
				}
				/*
				 * if (j + 1 + (i * col) < 10) { btnTag.setText(" " +
				 * String.valueOf(j + 1 + (i * col))); } else {
				 * btnTag.setText(String.valueOf(j + 1 + (i * col))); }
				 */
				b_row.addView(btnTag);
				puzzle[i][j] = btnTag;

			}
			linear.addView(b_row);
		}
		// End of make_buttons_dynamically
	}

	// check puzzle
	public void check_puzzle() {
		boolean end = true;

		for (int i = 0, number = 1; i < row; i++) {
			for (int j = 0; j < col; j++) {
				if (puzzle_array[i][j] != number) {
					end = false;
					break;
				}
				number++;
			}
		}

		if (end) {
			Log.d("MY_TAG", "왜냐하면 저는 되는 일이 하나도 없기 때문입니다");
			onBackPressed();
		}
	}// End of check puzzl

	// mix button array
	public void mix_array() {
		int count = 33333;
		int _row = row - 1; // current row
		int _col = col - 1; // current col

		while (--count > 0) {
			Random generator = new Random();
			int dir = generator.nextInt(4) + 1;

			switch (dir) {
			case 1:// up
				if (_row > 0) {
					int temp = puzzle_array[_row][_col];
					puzzle_array[_row][_col] = puzzle_array[_row - 1][_col];
					puzzle_array[_row - 1][_col] = temp;
					_row = _row - 1;
					Log.d("MY_RAND", "Dir" + String.valueOf(dir));
				}
				break;
			case 2:// down
				if (_row < row - 1) {
					int temp = puzzle_array[_row][_col];
					puzzle_array[_row][_col] = puzzle_array[_row + 1][_col];
					puzzle_array[_row + 1][_col] = temp;
					_row = _row + 1;
					Log.d("MY_RAND", "Dir" + String.valueOf(dir));
				}
				break;
			case 3:// left
				if (_col > 0) {
					int temp = puzzle_array[_row][_col];
					puzzle_array[_row][_col] = puzzle_array[_row][_col - 1];
					puzzle_array[_row][_col - 1] = temp;
					_col = _col - 1;
					Log.d("MY_RAND", "Dir" + String.valueOf(dir));
				}
				break;
			case 4:// right
				if (_col < col - 1) {
					int temp = puzzle_array[_row][_col];
					puzzle_array[_row][_col] = puzzle_array[_row][_col + 1];
					puzzle_array[_row][_col + 1] = temp;
					_col = _col + 1;
					Log.d("MY_RAND", "Dir" + String.valueOf(dir));
				}
				break;
			}
		}
		black_row = _row;
		black_col = _col;

	}// End of mix array

	// solve puzzle
	public void solve_puzzle(Button btn) {
		OnClickListener solve_puzzle = new OnClickListener() {
			@Override
			public void onClick(View v) {
				Log.d("MY_TAG", "퍼즐가나요");

				Button cur_btn = (Button) v;
				int cur_btn_num = Integer
						.parseInt(cur_btn.getText().toString());

				int sel_row = 0, sel_col = 0; // selected button's row and col

				// find selected button's row and col
				for (int i = 0; i < row; i++) {
					for (int j = 0; j < col; j++) {
						if (puzzle_array[i][j] == cur_btn_num) {
							sel_row = i;
							sel_col = j;
						}
					}
				}
				Log.d("MY_TAG",
						"current btn number: " + String.valueOf(cur_btn_num));

				// move button(black button)
				if ((black_row - sel_row == -1 && black_col - sel_col == 0/* up */)
						|| (black_row - sel_row == 1 && black_col - sel_col == 0/* down */)
						|| (black_row - sel_row == 0 && black_col - sel_col == -1/* left */)
						|| (black_row - sel_row == 0 && black_col - sel_col == 1/* right */)) {

					int temp = puzzle_array[sel_row][sel_col];
					puzzle_array[sel_row][sel_col] = puzzle_array[black_row][black_col];
					puzzle_array[black_row][black_col] = temp;

					Button temp_b = puzzle[sel_row][sel_col];
					puzzle[sel_row][sel_col] = puzzle[black_row][black_col];
					puzzle[black_row][black_col] = temp_b;

					black_row = sel_row;
					black_col = sel_col;

					// refresh
					// View.postInvalidate();
					//Log.d("MY_TAG", "KK__1");
					// linear.removeAllViews();

					for (int i = 0; i < row; i++) {
						for (int j = 0; j < col; j++) {
							Button btnTest = (Button) findViewById((j + 1 + (i * col)));
							btnTest.setText(String.valueOf(puzzle_array[i][j]));
							if (puzzle_array[i][j] == row * col) {
								btnTest.setBackgroundColor(Color.BLACK);
							} else {
								// btnTest.setBackgroundColor(Color.TRANSPARENT);
								btnTest.setBackgroundResource(android.R.drawable.btn_default);
							}
						}
					}

					/*
					 * for(int i=0, num=1 ; i<row;i++){ for(int j=0 ;j<col
					 * ;j++){
					 * 
					 * Log.d("MY_TAG", "remove"); Button dynamicButton =
					 * (Button)findViewById(DYNAMIC_VIEW_ID + num); num++;
					 * Log.d("MY_TAG","[" + String.valueOf(num)+"]" + "["
					 * +String.valueOf(puzzle_array[i][j])+"]" ); // View
					 * viewThatAlreadyHasAParent = view객체; //ViewGroup
					 * parentViewGroup =
					 * (ViewGroup)viewThatAlreadyHasAParent.getParent();
					 * ViewGroup parentViewGroup =
					 * (ViewGroup)linear.getParent();
					 * //parentViewGroup.removeView(parent를 제거하고 싶은 view 객체);
					 * parentViewGroup.removeView(dynamicButton);
					 * 
					 * //linear.removeView(dynamicButton);
					 * 
					 * } }
					 */
					/*
					 * for(int i=0 ; i< row ; i++){ LinearLayout dynamic_layout
					 * = (LinearLayout)findViewById(1000 + i);
					 * dynamic_layout.getParent().removeView(dynamic_layout);
					 * 
					 * linear.removeView(dynamic_layout);
					 * 
					 * //LinearView lv = getLinearView();
					 * //((ViewGroup)lv.getParent()).removeView(lv);
					 * 
					 * }
					 */

				} else {
					Log.d("MY_TAG", "의미없는 선택인 부분");
				}

				// 종료조건 check
				check_puzzle();
			}
		};
		btn.setOnClickListener(solve_puzzle);
	}

}