package ru.tonybo.pacman.game;

public class TileType {
	public static final int GHOST_HOME 	= 0x01;
	public static final int GHOST_PATH 	= 0x02;
	public static final int GHOST_TRAP 	= 0x04;
	public static final int GHOST_NO_UP	= 0x08;
	
	public static final int PACMAN_PATH = 0x10;
	
	public static final int TABLET 		= 0x20;
	public static final int ENERGIZER 	= 0x40;
	
	public static final int TUNNEL		= 0x80;
}