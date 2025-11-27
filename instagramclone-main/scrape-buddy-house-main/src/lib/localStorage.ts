import type { Post, Story, StoryItem, User, Comment } from '@/data/mockData';
import { posts as initialPosts, stories as initialStories, users as initialUsers } from '@/data/mockData';

const STORAGE_KEYS = {
  POSTS: 'instaclone_posts',
  STORIES: 'instaclone_stories',
  USERS: 'instaclone_users',
  CURRENT_USER: 'instaclone_current_user',
};

// Initialize localStorage with mock data if empty
export const initializeStorage = () => {
  if (!localStorage.getItem(STORAGE_KEYS.POSTS)) {
    localStorage.setItem(STORAGE_KEYS.POSTS, JSON.stringify(initialPosts));
  }
  if (!localStorage.getItem(STORAGE_KEYS.STORIES)) {
    localStorage.setItem(STORAGE_KEYS.STORIES, JSON.stringify(initialStories));
  }
  if (!localStorage.getItem(STORAGE_KEYS.USERS)) {
    localStorage.setItem(STORAGE_KEYS.USERS, JSON.stringify(initialUsers));
  }
  if (!localStorage.getItem(STORAGE_KEYS.CURRENT_USER)) {
    // Default current user is emma_creative
    localStorage.setItem(STORAGE_KEYS.CURRENT_USER, 'emma_creative');
  }
};

// Posts
export const getPosts = (): Post[] => {
  const stored = localStorage.getItem(STORAGE_KEYS.POSTS);
  return stored ? JSON.parse(stored) : [];
};

export const savePost = (post: Post) => {
  const posts = getPosts();
  posts.unshift(post);
  localStorage.setItem(STORAGE_KEYS.POSTS, JSON.stringify(posts));
  
  // Update user's post count
  const users = getUsers();
  const user = users.find(u => u.username === post.username);
  if (user) {
    updateUser(user.username, { posts: user.posts + 1 });
  }
};

export const updatePost = (postId: string, updates: Partial<Post>) => {
  const posts = getPosts();
  const index = posts.findIndex(p => p.id === postId);
  if (index !== -1) {
    posts[index] = { ...posts[index], ...updates };
    localStorage.setItem(STORAGE_KEYS.POSTS, JSON.stringify(posts));
  }
};

export const deletePost = (postId: string) => {
  const posts = getPosts();
  const filtered = posts.filter(p => p.id !== postId);
  localStorage.setItem(STORAGE_KEYS.POSTS, JSON.stringify(filtered));
  
  // Update user's post count
  const deletedPost = posts.find(p => p.id === postId);
  if (deletedPost) {
    const users = getUsers();
    const user = users.find(u => u.username === deletedPost.username);
    if (user && user.posts > 0) {
      updateUser(user.username, { posts: user.posts - 1 });
    }
  }
};

export const getPost = (postId: string): Post | undefined => {
  const posts = getPosts();
  return posts.find(p => p.id === postId);
};

export const addComment = (postId: string, comment: Comment) => {
  const posts = getPosts();
  const post = posts.find(p => p.id === postId);
  if (post) {
    post.comments.push(comment);
    localStorage.setItem(STORAGE_KEYS.POSTS, JSON.stringify(posts));
  }
};

// Stories
export const getStories = (): Story[] => {
  const stored = localStorage.getItem(STORAGE_KEYS.STORIES);
  const raw: any[] = stored ? JSON.parse(stored) : [];

  // Migrate older story shape (no items) and dedupe by userId
  const byUser = new Map<string, Story>();

  for (const r of raw) {
    const items = Array.isArray((r as any).items)
      ? (r as any).items
      : [
          {
            id: (r as any).id || `migrated_${Date.now()}_${Math.random().toString(36).slice(2, 7)}`,
            image: (r as any).image || (r as any).avatar,
            timestamp: (r as any).timestamp || 'Recently',
          },
        ];

    const s: Story = {
      id: String((r as any).id ?? `story_${(r as any).userId ?? Math.random()}`),
      userId: String((r as any).userId ?? ''),
      username: String((r as any).username ?? ''),
      avatar: String((r as any).avatar ?? ''),
      items,
      viewed: Boolean((r as any).viewed ?? false),
    };

    const existing = byUser.get(s.userId);
    if (existing) {
      // Merge items (newest first)
      const mergedItems = [...s.items, ...existing.items];
      byUser.set(s.userId, {
        ...existing,
        items: mergedItems,
        viewed: existing.viewed && s.viewed,
      });
    } else {
      byUser.set(s.userId, s);
    }
  }

  return Array.from(byUser.values());
};

export const saveStory = (story: Story) => {
  const stories = getStories();
  const existingStoryIndex = stories.findIndex(s => s.userId === story.userId);
  
  if (existingStoryIndex !== -1) {
    // Add new item to existing user's story
    stories[existingStoryIndex].items.unshift(...story.items);
    stories[existingStoryIndex].viewed = false;
  } else {
    // Create new story for user
    stories.unshift(story);
  }
  
  localStorage.setItem(STORAGE_KEYS.STORIES, JSON.stringify(stories));
};

export const markStoryAsViewed = (storyId: string) => {
  const stories = getStories();
  const story = stories.find(s => s.id === storyId);
  if (story) {
    story.viewed = true;
    localStorage.setItem(STORAGE_KEYS.STORIES, JSON.stringify(stories));
  }
};

// Users
export const getUsers = (): User[] => {
  const stored = localStorage.getItem(STORAGE_KEYS.USERS);
  return stored ? JSON.parse(stored) : [];
};

export const getUser = (username: string): User | undefined => {
  const users = getUsers();
  return users.find(u => u.username === username);
};

export const updateUser = (username: string, updates: Partial<User>) => {
  const users = getUsers();
  const index = users.findIndex(u => u.username === username);
  if (index !== -1) {
    users[index] = { ...users[index], ...updates };
    localStorage.setItem(STORAGE_KEYS.USERS, JSON.stringify(users));
  }
};

// Current user
export const getCurrentUser = (): string => {
  return localStorage.getItem(STORAGE_KEYS.CURRENT_USER) || 'emma_creative';
};

export const getCurrentUserData = (): User | undefined => {
  const username = getCurrentUser();
  return getUser(username);
};
