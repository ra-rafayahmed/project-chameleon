from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.common.exceptions import TimeoutException, NoSuchElementException
import json
import time
from datetime import datetime

class InstagramCloneScraper:
    def __init__(self, base_url="http://localhost:5173", debug=False):
        """Initialize the scraper with Chrome driver"""
        self.base_url = base_url
        self.debug = debug
        self.driver = webdriver.Chrome()
        self.driver.maximize_window()
        self.wait = WebDriverWait(self.driver, 10)
        
        if self.debug:
            print(f"🐛 Debug mode enabled")
            print(f"🌐 Base URL: {self.base_url}")
    
    def take_screenshot(self, name="debug"):
        """Take a screenshot for debugging"""
        if self.debug:
            filename = f"{name}_{datetime.now().strftime('%H%M%S')}.png"
            self.driver.save_screenshot(filename)
            print(f"📸 Screenshot saved: {filename}")
        
    def scrape_posts_from_feed(self):
        """Scrape all posts from the home feed"""
        print("Scraping posts from feed...")
        self.driver.get(self.base_url)
        time.sleep(3)  # Wait for page load
        
        posts = []
        
        # Scroll down to load more posts
        self.driver.execute_script("window.scrollTo(0, document.body.scrollHeight);")
        time.sleep(2)
        
        # Try multiple selectors to find posts
        post_elements = []
        try:
            post_elements = self.driver.find_elements(By.CSS_SELECTOR, '[data-section="feed"] > div')
        except:
            pass
        
        if not post_elements:
            try:
                post_elements = self.driver.find_elements(By.XPATH, '//section[@data-section="feed"]/div')
            except:
                pass
        
        if not post_elements:
            # Fallback: find all card-like containers
            post_elements = self.driver.find_elements(By.XPATH, '//main//div[.//img and .//p]')
        
        print(f"Found {len(post_elements)} post elements")
        
        for i, post_el in enumerate(post_elements):
            try:
                post_data = {'post_number': i + 1}
                
                # Get username
                try:
                    username_link = post_el.find_element(By.CSS_SELECTOR, 'a[href*="/profile/"]')
                    post_data['username'] = username_link.text.strip()
                except:
                    post_data['username'] = 'Unknown'
                
                # Get image
                try:
                    img = post_el.find_element(By.TAG_NAME, 'img')
                    post_data['image_src'] = img.get_attribute('src')
                except:
                    post_data['image_src'] = None
                
                # Get caption - look for text paragraphs
                try:
                    paragraphs = post_el.find_elements(By.TAG_NAME, 'p')
                    for p in paragraphs:
                        text = p.text.strip()
                        if len(text) > 20 and not text.endswith('ago') and 'likes' not in text:
                            post_data['caption'] = text
                            break
                except:
                    post_data['caption'] = None
                
                # Get likes
                try:
                    likes_el = post_el.find_element(By.XPATH, './/p[contains(text(), "like")]')
                    post_data['likes'] = likes_el.text.strip()
                except:
                    post_data['likes'] = '0'
                
                # Get timestamp
                try:
                    time_els = post_el.find_elements(By.XPATH, './/p[contains(text(), "ago") or contains(text(), "hour") or contains(text(), "day")]')
                    if time_els:
                        post_data['timestamp'] = time_els[0].text.strip()
                except:
                    post_data['timestamp'] = 'Unknown'
                
                # Get comments count
                try:
                    comments = post_el.find_elements(By.XPATH, './/*[contains(@data-comment-id, "")]')
                    post_data['comments_count'] = len(comments)
                except:
                    post_data['comments_count'] = 0
                
                posts.append(post_data)
                print(f"✓ Scraped post #{i+1} by {post_data['username']}")
            except Exception as e:
                print(f"✗ Error scraping post #{i+1}: {e}")
                continue
        
        return posts
    
    def scrape_stories(self):
        """Scrape all visible stories"""
        print("Scraping stories...")
        self.driver.get(self.base_url)
        time.sleep(3)
        
        stories = []
        
        # Try multiple ways to find stories
        story_elements = []
        try:
            story_elements = self.driver.find_elements(By.CSS_SELECTOR, '[data-section="stories"] div')
        except:
            pass
        
        if not story_elements:
            try:
                story_elements = self.driver.find_elements(By.XPATH, '//section[@data-section="stories"]//div[@class="flex flex-col items-center gap-1 cursor-pointer group"]')
            except:
                pass
        
        if not story_elements:
            # Fallback: look for avatar images near top of page
            try:
                story_elements = self.driver.find_elements(By.XPATH, '//main//section[1]//div[contains(@class, "flex")]//img[@alt]/..')
            except:
                pass
        
        print(f"Found {len(story_elements)} story elements")
        
        for i, story_el in enumerate(story_elements):
            try:
                story_data = {'story_number': i + 1}
                
                # Get username
                try:
                    username_el = story_el.find_element(By.TAG_NAME, 'p')
                    story_data['username'] = username_el.text.strip()
                except:
                    try:
                        img = story_el.find_element(By.TAG_NAME, 'img')
                        story_data['username'] = img.get_attribute('alt')
                    except:
                        story_data['username'] = f'User_{i+1}'
                
                # Get avatar
                try:
                    avatar_img = story_el.find_element(By.TAG_NAME, 'img')
                    story_data['avatar_src'] = avatar_img.get_attribute('src')
                except:
                    story_data['avatar_src'] = None
                
                # Check if viewed (gradient ring vs plain ring)
                try:
                    html = story_el.get_attribute('outerHTML')
                    story_data['has_gradient'] = 'gradient' in html.lower()
                    story_data['viewed'] = not story_data['has_gradient']
                except:
                    story_data['viewed'] = False
                
                # Check if it's "Your Story"
                story_data['is_own_story'] = 'your story' in story_data['username'].lower()
                
                stories.append(story_data)
                print(f"✓ Scraped story #{i+1} from {story_data['username']}")
            except Exception as e:
                print(f"✗ Error scraping story #{i+1}: {e}")
                continue
        
        return stories
    
    def scrape_explore_page(self):
        """Scrape posts from explore page"""
        print("Scraping explore page...")
        self.driver.get(f"{self.base_url}/explore")
        time.sleep(3)
        
        # Scroll to load more content
        for _ in range(3):
            self.driver.execute_script("window.scrollTo(0, document.body.scrollHeight);")
            time.sleep(1)
        
        posts = []
        
        # Try multiple selectors
        post_elements = []
        try:
            post_elements = self.driver.find_elements(By.CSS_SELECTOR, '[data-section="explore-grid"] a')
        except:
            pass
        
        if not post_elements:
            try:
                post_elements = self.driver.find_elements(By.XPATH, '//section[@data-section="explore-grid"]//a')
            except:
                pass
        
        if not post_elements:
            # Fallback: find all links with images in explore page
            post_elements = self.driver.find_elements(By.XPATH, '//main//a[.//img]')
        
        print(f"Found {len(post_elements)} explore posts")
        
        for i, post_el in enumerate(post_elements):
            try:
                post_data = {
                    'post_number': i + 1,
                    'link': post_el.get_attribute('href')
                }
                
                # Get post ID from link
                if post_data['link'] and '/post/' in post_data['link']:
                    post_data['post_id'] = post_data['link'].split('/post/')[-1]
                else:
                    post_data['post_id'] = f'unknown_{i}'
                
                # Get image
                try:
                    img = post_el.find_element(By.TAG_NAME, 'img')
                    post_data['image_src'] = img.get_attribute('src')
                    post_data['alt_text'] = img.get_attribute('alt')
                except:
                    post_data['image_src'] = None
                    post_data['alt_text'] = None
                
                posts.append(post_data)
                print(f"✓ Scraped explore post #{i+1} (ID: {post_data['post_id']})")
            except Exception as e:
                print(f"✗ Error scraping explore post #{i+1}: {e}")
                continue
        
        return posts
    
    def scrape_user_profile(self, username):
        """Scrape a user's profile information"""
        print(f"Scraping profile: {username}")
        self.driver.get(f"{self.base_url}/profile/{username}")
        time.sleep(2)
        
        try:
            profile_data = {
                'username': self.driver.find_element(By.CSS_SELECTOR, 'h1.text-2xl').text,
                'full_name': None,
                'bio': None,
                'verified': bool(self.driver.find_elements(By.XPATH, '//span[contains(text(), "Verified")]')),
                'stats': {}
            }
            
            # Get profile stats
            stats_section = self.driver.find_element(By.CSS_SELECTOR, '[data-stats]')
            stat_divs = stats_section.find_elements(By.CSS_SELECTOR, 'div, button')
            
            for stat in stat_divs:
                text = stat.text
                if 'posts' in text.lower():
                    profile_data['stats']['posts'] = text.split()[0]
                elif 'followers' in text.lower():
                    profile_data['stats']['followers'] = text.split()[0]
                elif 'following' in text.lower():
                    profile_data['stats']['following'] = text.split()[0]
            
            # Get bio
            try:
                bio_elements = self.driver.find_elements(By.CSS_SELECTOR, 'p.text-foreground.whitespace-pre-line')
                if bio_elements:
                    profile_data['bio'] = bio_elements[0].text
                
                full_name_el = self.driver.find_element(By.CSS_SELECTOR, 'p.font-semibold.text-foreground')
                profile_data['full_name'] = full_name_el.text
            except:
                pass
            
            # Get user's posts
            profile_data['posts'] = []
            post_elements = self.driver.find_elements(By.CSS_SELECTOR, '[data-section="profile-posts"] img')
            
            for post_img in post_elements:
                profile_data['posts'].append({
                    'image_src': post_img.get_attribute('src'),
                    'alt_text': post_img.get_attribute('alt')
                })
            
            print(f"✓ Scraped profile {username} with {len(profile_data['posts'])} posts")
            return profile_data
            
        except Exception as e:
            print(f"✗ Error scraping profile {username}: {e}")
            return None
    
    def scrape_post_details(self, post_id):
        """Scrape detailed information from a specific post"""
        print(f"Scraping post details: {post_id}")
        self.driver.get(f"{self.base_url}/post/{post_id}")
        time.sleep(2)
        
        try:
            post_data = {
                'post_id': post_id,
                'username': self.driver.find_element(By.CSS_SELECTOR, 'p.text-sm.font-semibold').text,
                'caption': None,
                'likes': None,
                'comments': [],
                'timestamp': None,
                'location': None
            }
            
            # Get location if available
            try:
                location_el = self.driver.find_element(By.XPATH, '//p[@class="text-xs text-muted-foreground"]')
                post_data['location'] = location_el.text
            except:
                pass
            
            # Get caption
            try:
                caption_el = self.driver.find_element(By.XPATH, '//span[@class="text-foreground"]')
                post_data['caption'] = caption_el.text
            except:
                pass
            
            # Get likes
            try:
                likes_el = self.driver.find_element(By.XPATH, '//p[contains(text(), "likes")]')
                post_data['likes'] = likes_el.text
            except:
                pass
            
            # Get comments
            comment_elements = self.driver.find_elements(By.CSS_SELECTOR, '[data-comment-id]')
            for comment_el in comment_elements:
                try:
                    comment_data = {
                        'comment_id': comment_el.get_attribute('data-comment-id'),
                        'username': comment_el.find_element(By.CSS_SELECTOR, 'a').text,
                        'text': comment_el.find_element(By.XPATH, './/span[@class="text-foreground"]').text,
                        'timestamp': comment_el.find_element(By.CSS_SELECTOR, 'p.text-xs.text-muted-foreground').text
                    }
                    post_data['comments'].append(comment_data)
                except:
                    continue
            
            print(f"✓ Scraped post {post_id} with {len(post_data['comments'])} comments")
            return post_data
            
        except Exception as e:
            print(f"✗ Error scraping post details: {e}")
            return None
    
    def scrape_messages_notes(self):
        """Scrape notes from the messages page"""
        print("Scraping messages and notes...")
        self.driver.get(f"{self.base_url}/messages")
        time.sleep(2)
        
        data = {
            'notes': [],
            'chats': []
        }
        
        # Scrape notes
        try:
            note_elements = self.driver.find_elements(By.XPATH, '//button[contains(@class, "flex flex-col items-center")]')
            for note_el in note_elements:
                try:
                    username = note_el.find_element(By.CSS_SELECTOR, 'span.text-xs').text
                    note_text_el = note_el.find_elements(By.CSS_SELECTOR, 'div.bg-accent')
                    
                    note_data = {
                        'username': username,
                        'is_own_note': 'Your note' in note_el.text
                    }
                    
                    if note_text_el:
                        note_data['text'] = note_text_el[0].text
                    
                    data['notes'].append(note_data)
                    print(f"✓ Scraped note from {username}")
                except:
                    continue
        except Exception as e:
            print(f"✗ Error scraping notes: {e}")
        
        # Scrape chat list
        try:
            chat_elements = self.driver.find_elements(By.XPATH, '//button[contains(@class, "w-full flex items-center")]')
            for chat_el in chat_elements:
                try:
                    username = chat_el.find_element(By.CSS_SELECTOR, 'p.text-sm').text
                    last_message = chat_el.find_elements(By.CSS_SELECTOR, 'p.text-sm')[1].text
                    timestamp = chat_el.find_element(By.CSS_SELECTOR, 'span.text-xs').text
                    has_unread = bool(chat_el.find_elements(By.CSS_SELECTOR, 'div.bg-primary.rounded-full'))
                    
                    chat_data = {
                        'username': username,
                        'last_message': last_message,
                        'timestamp': timestamp,
                        'unread': has_unread
                    }
                    
                    data['chats'].append(chat_data)
                    print(f"✓ Scraped chat with {username}")
                except:
                    continue
        except Exception as e:
            print(f"✗ Error scraping chats: {e}")
        
        return data
    
    def scrape_search_results(self, query):
        """Scrape search results for a query"""
        print(f"Scraping search results for: {query}")
        self.driver.get(f"{self.base_url}/explore")
        time.sleep(2)
        
        # Find and fill search input
        search_input = self.driver.find_element(By.CSS_SELECTOR, 'input[type="search"]')
        search_input.clear()
        search_input.send_keys(query)
        time.sleep(2)  # Wait for results
        
        results = {
            'query': query,
            'accounts': [],
            'posts': []
        }
        
        # Scrape account results
        try:
            account_elements = self.driver.find_elements(By.XPATH, '//a[contains(@href, "/profile/")]')
            for account_el in account_elements[:5]:  # Limit to first 5
                try:
                    username = account_el.find_element(By.CSS_SELECTOR, 'p.font-semibold').text
                    full_name = account_el.find_element(By.CSS_SELECTOR, 'p.text-sm.text-muted-foreground').text
                    
                    results['accounts'].append({
                        'username': username,
                        'full_name': full_name
                    })
                    print(f"✓ Found account: {username}")
                except:
                    continue
        except:
            pass
        
        return results
    
    def scrape_all(self):
        """Comprehensive scraping of the entire application"""
        print("\n" + "="*50)
        print("STARTING COMPREHENSIVE SCRAPE")
        print("="*50 + "\n")
        
        all_data = {
            'timestamp': datetime.now().isoformat(),
            'feed_posts': [],
            'stories': [],
            'explore_posts': [],
            'profiles': [],
            'messages_data': {},
            'post_details': []
        }
        
        # Take initial screenshot
        if self.debug:
            self.take_screenshot("00_start")
        
        # 1. Scrape feed posts
        try:
            all_data['feed_posts'] = self.scrape_posts_from_feed()
            if self.debug:
                self.take_screenshot("01_feed")
        except Exception as e:
            print(f"❌ Error scraping feed: {e}")
        
        # 2. Scrape stories
        try:
            all_data['stories'] = self.scrape_stories()
            if self.debug:
                self.take_screenshot("02_stories")
        except Exception as e:
            print(f"❌ Error scraping stories: {e}")
        
        # 3. Scrape explore page
        try:
            all_data['explore_posts'] = self.scrape_explore_page()
            if self.debug:
                self.take_screenshot("03_explore")
        except Exception as e:
            print(f"❌ Error scraping explore: {e}")
        
        # 4. Scrape messages and notes
        try:
            all_data['messages_data'] = self.scrape_messages_notes()
            if self.debug:
                self.take_screenshot("04_messages")
        except Exception as e:
            print(f"❌ Error scraping messages: {e}")
        
        # 5. Scrape profiles (from stories/posts)
        usernames = set()
        for post in all_data['feed_posts']:
            if 'username' in post and post['username'] != 'Unknown':
                usernames.add(post['username'])
        for story in all_data['stories']:
            if 'username' in story and not story.get('is_own_story', False):
                usernames.add(story['username'])
        
        for username in list(usernames)[:5]:  # Limit to first 5 users
            try:
                profile_data = self.scrape_user_profile(username)
                if profile_data:
                    all_data['profiles'].append(profile_data)
            except Exception as e:
                print(f"❌ Error scraping profile {username}: {e}")
        
        print("\n" + "="*50)
        print("SCRAPING COMPLETE")
        print("="*50)
        print(f"Feed Posts: {len(all_data['feed_posts'])}")
        print(f"Stories: {len(all_data['stories'])}")
        print(f"Explore Posts: {len(all_data['explore_posts'])}")
        print(f"Profiles: {len(all_data['profiles'])}")
        print(f"Notes: {len(all_data['messages_data'].get('notes', []))}")
        print(f"Chats: {len(all_data['messages_data'].get('chats', []))}")
        
        return all_data
    
    def save_to_json(self, data, filename='instagram_clone_data.json'):
        """Save scraped data to JSON file"""
        with open(filename, 'w', encoding='utf-8') as f:
            json.dump(data, f, indent=2, ensure_ascii=False)
        print(f"\n✓ Data saved to {filename}")
    
    def close(self):
        """Close the browser"""
        self.driver.quit()
        print("Browser closed")


# Example usage
if __name__ == "__main__":
    # Initialize scraper with DEBUG MODE (adjust URL if your app runs on different port)
    scraper = InstagramCloneScraper(base_url="http://localhost:8080", debug=True)
    
    try:
        # Option 1: Scrape everything
        all_data = scraper.scrape_all()
        scraper.save_to_json(all_data)
        
        # Option 2: Scrape specific components
        # posts = scraper.scrape_posts_from_feed()
        # stories = scraper.scrape_stories()
        # profile = scraper.scrape_user_profile('emma_creative')
        # messages = scraper.scrape_messages_notes()
        # search_results = scraper.scrape_search_results('sunset')
        
    finally:
        scraper.close()