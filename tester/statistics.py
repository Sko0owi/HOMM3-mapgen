from collections import Counter


win_counter = Counter()

def get_user_input_with_default(prompt, default_value=3):
    """
    Get user input with a default value.
    
    Args:
        prompt (str): Message to show to user
        default_value (int): Default value if user just presses Enter
        
    Returns:
        int: User input value or default value
    """
    user_input = input(f"{prompt} (default={default_value}): ").strip()
    if user_input == "":
        return default_value
    try:
        return int(user_input)
    except ValueError:
        print(f"Invalid input. Using default value: {default_value}")
        return default_value
    
def analyze_player_wins(filename):
    """
    Analyze a file to find mentions of player wins and create statistics without using regex.
    
    Args:
        filename (str): Path to the file to analyze
    
    Returns:
        dict: Dictionary containing win statistics for each player
    """
    
    try:
        with open(filename, 'r', encoding='utf-8') as file:
            for line in file:
                # Skip empty lines
                if not line.strip():
                    continue
                    
                # Check if the line contains "won"
                if "won!" not in line:
                    continue
                
                # Split the line into parts
                # Find the last occurrence of "Player" before "won!"
                winner_part = line[line.rfind("Player"):line.find("won")].strip()
                
                # Extract the winner information if it matches the expected format
                if "(" in winner_part and ")" in winner_part:
                    # Get everything between "Player" and "won"
                    winner = winner_part.strip()
                    win_counter[winner] += 1
    
    except FileNotFoundError:
        return f"Error: File '{filename}' not found."
    except Exception as e:
        return f"Error occurred: {str(e)}"

def print_statistics(stats):
    """
    Print the statistics in a formatted way.
    
    Args:
        stats (dict): Dictionary containing player statistics
    """
    if isinstance(stats, str):  # Error message
        print(stats)
        return
    
    print("\nPlayer Win Statistics:")
    print("-" * 40)
    
    if not stats:
        print("No matching patterns found in the file.")
        return
    
    totalWins = sum(stats.values())

        
    for player, wins in sorted(stats.items(), key=lambda x: (-x[1], x[0])):
        print(f"{player}: {wins} {'win' if wins == 1 else 'wins'} {wins/totalWins*100:.2f}% WR")

# Example usage
if __name__ == "__main__":
    num_of_games = get_user_input_with_default("Enter number of games", 3)
    num_of_batches = get_user_input_with_default("Enter number of batches", 3)

    for i in range(num_of_batches):
        for j in range(num_of_games):
            filename = f"./log_{i + 1}_{j+1}/last_lines.txt"
            analyze_player_wins(filename)
    
    stats = dict(win_counter)
    print_statistics(stats)