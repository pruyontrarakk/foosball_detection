#!/usr/bin/env python3
# process_ball_csv.py
#
# Usage:
#   python process_ball_csv.py \
#       --csv ball_pixels.csv \
#       --out ball_traj_metric.csv \
#       --img-corners "x1,y1;x2,y2;x3,y3;x4,y4" \
#       --size-m 1.20 0.68 \
#       --fps 60

import argparse, numpy as np, pandas as pd, cv2 as cv

def parse_pts(s):
    pts = []
    for p in s.split(';'):
        x,y = p.split(',')
        pts.append([float(x), float(y)])
    return np.array(pts, dtype=np.float32)

def smooth_xy(x, win=9):
    # simple moving average (odd window), fallback if short
    if len(x) < win or win < 3:
        return x.copy()
    k = win // 2
    pad = np.pad(x, (k,k), mode='edge')
    ker = np.ones(win)/win
    return np.convolve(pad, ker, mode='valid')

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--csv', required=True)
    ap.add_argument('--out', required=True)
    ap.add_argument('--img-corners', required=True,
                    help='four pixel points in order: TL;TR;BR;BL or any consistent clockwise order')
    ap.add_argument('--size-m', nargs=2, type=float, required=True, help='W H (meters)')
    ap.add_argument('--fps', type=float, default=60.0)
    ap.add_argument('--resample-hz', type=float, default=100.0)
    ap.add_argument('--smooth-win', type=int, default=9)
    args = ap.parse_args()

    df = pd.read_csv(args.csv)            # t_sec, u_px, v_px
    t  = df['t_sec'].to_numpy()
    u  = df['u_px'].to_numpy()
    v  = df['v_px'].to_numpy()

    # build homography pixels->meters
    pts_img = parse_pts(args.img_corners)            # 4x2
    W, H = args.size_m
    pts_m  = np.array([[0,0],[W,0],[W,H],[0,H]], dtype=np.float32)  # match your img-corners order
    Hmat, _ = cv.findHomography(pts_img, pts_m, method=cv.RANSAC)

    # map all (u,v) -> (x,y) meters
    ones = np.ones_like(u)
    pts  = np.stack([u, v, ones], axis=0)           # 3xN
    XYh  = Hmat @ pts                                # 3xN
    X    = XYh[0]/XYh[2]
    Y    = XYh[1]/XYh[2]

    # optional smoothing (position)
    Xs = smooth_xy(X, win=args.smooth_win)
    Ys = smooth_xy(Y, win=args.smooth_win)

    # resample at fixed rate (good for MuJoCo step sync)
    dt_target = 1.0/args.resample_hz
    t_new = np.arange(t[0], t[-1], dt_target)
    Xr = np.interp(t_new, t, Xs)
    Yr = np.interp(t_new, t, Ys)

    # finite-diff velocities
    Vx = np.gradient(Xr, dt_target)
    Vy = np.gradient(Yr, dt_target)

    out = pd.DataFrame({
        't_sec': t_new,
        'x_m': Xr, 'y_m': Yr,
        'vx_mps': Vx, 'vy_mps': Vy
    })
    out.to_csv(args.out, index=False)
    print(f'Wrote {args.out}  ({len(out)} rows)')

if __name__ == '__main__':
    main()
