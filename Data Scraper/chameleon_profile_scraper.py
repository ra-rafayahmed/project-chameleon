from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
import time
import json
import os

URL = "https://project-chameleon.vercel.app/"
USERNAME_TO_SEARCH = "alex_photo"

# --- Setup output folder ---
OUTPUT_FOLDER = "output_profiles"
os.makedirs(OUTPUT_FOLDER, exist_ok=True)
JSON_FILE = os.path.join(OUTPUT_FOLDER, f"{USERNAME_TO_SEARCH}_profile.json")

options = webdriver.ChromeOptions()
options.add_argument("--start-maximized")
driver = webdriver.Chrome(options=options)
wait = WebDriverWait(driver, 15)

def scrape_user_list(button_xpath):
    """Click button (followers/following) and scrape usernames from modal."""
    users = []
    try:
        button = driver.find_element(By.XPATH, button_xpath)
        button.click()
        time.sleep(1)  # wait for modal to open

        modal = wait.until(
            EC.presence_of_element_located((By.XPATH, "//div[@role='dialog' and @data-state='open']"))
        )

        # Scroll modal to load all users (if needed)
        last_height = driver.execute_script("return arguments[0].scrollHeight", modal)
        while True:
            driver.execute_script("arguments[0].scrollTop = arguments[0].scrollHeight", modal)
            time.sleep(1)
            new_height = driver.execute_script("return arguments[0].scrollHeight", modal)
            if new_height == last_height:
                break
            last_height = new_height

        user_links = modal.find_elements(By.XPATH, ".//a[contains(@href, '/profile/')]")
        for u in user_links:
            username = u.find_element(By.XPATH, ".//p[contains(@class,'font-semibold')]").text
            users.append(username)

        # Close modal
        close_btn = modal.find_element(By.XPATH, ".//button[@aria-label='Close' or @type='button']")
        close_btn.click()
        time.sleep(1)

    except Exception as e:
        print(f"Error scraping user list: {e}")

    return users

try:
    # Open root page
    driver.get(URL)
    wait.until(EC.presence_of_element_located((By.TAG_NAME, "body")))

    # Click Explore button
    explore_btn = wait.until(
        EC.element_to_be_clickable((By.XPATH, "//a[contains(@href, '/explore')]"))
    )
    explore_btn.click()
    print("Explore page opened successfully!")

    # Wait for search bar and search
    search_input = wait.until(
        EC.element_to_be_clickable((By.CSS_SELECTOR, "input[type='search']"))
    )
    search_input.click()
    search_input.send_keys(USERNAME_TO_SEARCH)
    search_input.send_keys(Keys.RETURN)
    print(f"Searched for username: {USERNAME_TO_SEARCH}")

    # Enter first profile
    first_result = wait.until(
        EC.element_to_be_clickable(
            (By.XPATH, "//div[contains(@class,'space-y-3')]//a[contains(@href, '/profile/')]")
        )
    )
    first_result.click()
    print(f"Entered profile of {USERNAME_TO_SEARCH}!")
    time.sleep(2)  # allow SPA to render posts

    profile_data = {"username": USERNAME_TO_SEARCH}

    # --- Scrape bio ---
    try:
        bio_elem = driver.find_element(By.XPATH, "//div[p[contains(@class,'font-semibold')]]/p[2]")
        profile_data["bio"] = bio_elem.text
    except:
        profile_data["bio"] = ""

    # --- Scrape stats ---
    try:
        posts_count = driver.find_element(By.XPATH, "//div[@data-stats='true']/div/span").text
        followers_count = driver.find_element(By.XPATH, "//div[@data-stats='true']/button[1]/span").text
        following_count = driver.find_element(By.XPATH, "//div[@data-stats='true']/button[2]/span").text

        profile_data["posts_count"] = posts_count
        profile_data["followers_count"] = followers_count
        profile_data["following_count"] = following_count
    except:
        profile_data["posts_count"] = profile_data["followers_count"] = profile_data["following_count"] = "0"

    # --- Scrape posts ---
    posts = driver.find_elements(By.XPATH, "//div[contains(@class,'aspect-square')]//a")
    scraped_posts = []
    for post in posts:
        try:
            post_url = post.get_attribute("href")
            img = post.find_element(By.TAG_NAME, "img")
            img_url = img.get_attribute("src")
            caption = img.get_attribute("alt")
            scraped_posts.append({
                "post_url": post_url,
                "img_url": img_url,
                "caption": caption
            })
        except Exception as e:
            print(f"Error scraping a post: {e}")
    profile_data["posts"] = scraped_posts

    # --- Scrape followers and following list ---
    followers_xpath = "//button[span[contains(text(),'followers')]]"
    following_xpath = "//button[span[contains(text(),'following')]]"

    profile_data["followers_list"] = scrape_user_list(followers_xpath)
    profile_data["following_list"] = scrape_user_list(following_xpath)

    # --- Save to JSON ---
    with open(JSON_FILE, "w", encoding="utf-8") as f:
        json.dump(profile_data, f, ensure_ascii=False, indent=4)

    print(f"Scraping completed! Data saved to {JSON_FILE}")

finally:
    input("Press Enter to close browser...")
    driver.quit()
